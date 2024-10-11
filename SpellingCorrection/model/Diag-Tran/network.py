from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tensorflow.contrib import rnn
from tensorflow.contrib import layers
import numpy as np
from modules import multihead_attention_diag, feedforward

class Transformer(object):
    def __init__(self, seq_len, input_vocab_size, masked_input_vocab_size, 
                 tag_vocab_size, emb_size, hid_dim, num_cls, 
                 num_heads=4, num_blocks=1, num_lm_layers=2, lr=0.001, dev_info='/gpu:0', max_model_to_keep=2):
        self.seq_len = seq_len
        self.input_vocab_size = input_vocab_size
        self.masked_input_vocab_size = masked_input_vocab_size
        self.tag_vocab_size = tag_vocab_size
        self.emb_size = emb_size
        self.hid_size = hid_dim
        self.num_heads = num_heads
        self.num_blocks = num_blocks
        self.num_lm_layers = num_lm_layers
        self.num_cls = num_cls
        self.lr = lr
        self.inputs = tf.placeholder(tf.int32, [None, self.seq_len], name='pinyin_inputs')
        self.masked_inputs = tf.placeholder(tf.int32, [None, self.seq_len], name='hanzi_inputs')
        self.input_tags = tf.placeholder(tf.int32, [None, self.seq_len], name='seg_tags')
        self.negs = tf.placeholder(tf.int32, [None, self.seq_len], name='negs')
        self.targets = tf.placeholder(tf.int64, [None, self.seq_len], name='targets')
        self.dropout_keep_prob = tf.placeholder(tf.float32, name='dropout_keep_prob')
        self.sequence_length = tf.reduce_sum(tf.sign(self.inputs), axis=1)
        self.sequence_length = tf.cast(self.sequence_length, tf.int32)

        # initialization
        with tf.device(dev_info):
            self.all_input_emb = tf.get_variable(name="all_input_emb", shape=[self.input_vocab_size, self.emb_size], initializer=tf.truncated_normal_initializer(stddev=(1.0 / np.sqrt(self.emb_size))))
            self.all_masked_input_emb = tf.get_variable(name="all_masked_input_emb", shape=[self.masked_input_vocab_size, self.emb_size], initializer=tf.truncated_normal_initializer(stddev=(1.0 / np.sqrt(self.emb_size))))
            self.all_input_tag_emb = tf.get_variable(name="all_input_tag_emb", shape=[self.tag_vocab_size, self.emb_size], initializer=tf.truncated_normal_initializer(stddev=(1.0 / np.sqrt(self.emb_size))))
            self.all_output_emb = tf.get_variable(name="all_output_emb", shape=[self.num_cls, self.emb_size], initializer=tf.truncated_normal_initializer(stddev=(1.0 / np.sqrt(self.emb_size))))
            self.all_output_emb = tf.concat((tf.zeros(shape=[1, self.emb_size]),self.all_output_emb[1:, :]), 0)
            self.all_pos_emb = tf.get_variable(name="all_pos_emb", shape=[self.seq_len, self.emb_size], initializer=tf.truncated_normal_initializer(stddev=(1.0 / np.sqrt(self.emb_size))))    

            self.inputs_emb = tf.nn.embedding_lookup(self.all_input_emb, self.inputs)
            self.masked_inputs_emb = tf.nn.embedding_lookup(self.all_masked_input_emb, self.masked_inputs)
            self.input_tags_emb = tf.nn.embedding_lookup(self.all_input_tag_emb, self.input_tags)
            self.negs_emb = tf.nn.embedding_lookup(self.all_output_emb, self.negs)
            self.outputs_emb = tf.nn.embedding_lookup(self.all_output_emb, self.targets)
            # Add position embedding
            self.pos_emb = tf.nn.embedding_lookup(self.all_pos_emb, tf.tile(tf.expand_dims(tf.range(tf.shape(self.targets[:, 1:])[1]), 0), [tf.shape(self.targets)[0], 1]))
            # self.outputs_emb *= key_masks

            self.inputs_emb_reversed = tf.reverse_sequence(self.inputs_emb, self.sequence_length, seq_dim=1, batch_dim=0)
            self.masked_inputs_emb_reversed = tf.reverse_sequence(self.masked_inputs_emb, self.sequence_length, seq_dim=1, batch_dim=0)
            self.input_tags_emb_reversed = tf.reverse_sequence(self.input_tags_emb, self.sequence_length, seq_dim=1, batch_dim=0)
            self.negs_emb_reversed = tf.reverse_sequence(self.negs_emb, self.sequence_length, seq_dim=1, batch_dim=0)
            self.outputs_emb_reversed = tf.reverse_sequence(self.outputs_emb, self.sequence_length, seq_dim=1, batch_dim=0)

        # self.sequence_length -= 1 # let the length - 1

        # network definition
        with tf.device(dev_info):
            # Slice
            self.inputs_emb_sliced = self.inputs_emb[:, 1:, :]
            self.input_tags_emb_sliced = self.input_tags_emb[:, 1:, :]
            self.negs_emb_sliced = self.negs_emb[:, 1:, :]
            self.outputs_emb_sliced = self.outputs_emb[:, 1:, :]
            key_masks = tf.expand_dims(tf.sign(tf.reduce_sum(tf.abs(self.outputs_emb_sliced), axis=-1)), -1)
            # Concat the context information, pinyin, boundary and negative words
            params = {"filters": self.emb_size, "kernel_size": 1,
                      "activation": None, "use_bias": True, "kernel_initializer": tf.contrib.layers.xavier_initializer()}
            self.linear = tf.layers.Conv1D(**params)
            self.concat_neg_inputs = tf.concat([self.inputs_emb_sliced, self.negs_emb_sliced], 2) # [batch_size, seq_len, 3 * hid_size]
            self.concat_gold_inputs = tf.concat([self.inputs_emb_sliced, self.outputs_emb_sliced], 2)

            with tf.variable_scope("linear") as scope:
                self.concat_neg_inputs = self.linear(self.concat_neg_inputs)
            with tf.variable_scope(scope, reuse=True):
                self.concat_gold_inputs = self.linear(self.concat_gold_inputs)
            
            #Add Position embedding and mask
            self.concat_neg_inputs += self.pos_emb
            self.concat_gold_inputs += self.pos_emb

            self.concat_neg_inputs *= key_masks
            self.concat_gold_inputs *= key_masks

            # bi-directional transformer
            def context_layer(layer_name, neg_inputs, gold_inputs, hid_size_multiple=1):
                with tf.variable_scope(layer_name):
                    queries = neg_inputs
                    keys = gold_inputs
                    for i in range(self.num_blocks):
                        with tf.variable_scope("transformer_%d"%(i)):
                            # lstm_cell_fw = tf.nn.rnn_cell.LSTMCell(hid_size_multiple * self.hid_size)
                            atten_out = multihead_attention_diag(queries=queries, 
                                    keys=keys, 
                                    num_units=hid_size_multiple * self.hid_size, 
                                    num_heads=self.num_heads, 
                                    dropout_rate=1.0-self.dropout_keep_prob,
                                    scope="self_attention",
                                    causality=True)
                            queries = feedforward(atten_out, num_units=[4*hid_size_multiple * self.hid_size, hid_size_multiple * self.hid_size])

                return queries

            self.outputs = context_layer('transformer', self.concat_neg_inputs, self.concat_gold_inputs, 1) # [batch_size, seq_len, hid_size]
            self.outputs_sliced = tf.reverse_sequence(tf.reverse_sequence(self.outputs, self.sequence_length-1, seq_dim=1, batch_dim=0)[:, 1:], self.sequence_length - 2, seq_dim=1, batch_dim=0)
            softmax_W = self.all_output_emb
            softmax_b = tf.get_variable('softmax_b', [self.num_cls], initializer=tf.constant_initializer(0.0))
            self.logits = tf.tensordot(self.outputs_sliced, softmax_W, axes = [[2], [1]]) + softmax_b

            # max entropy loss
            sliced_targets = self.targets[:, 1:]
            sliced_targets = tf.reverse_sequence(tf.reverse_sequence(sliced_targets, self.sequence_length-1, seq_dim=1, batch_dim=0)[:, 1:], self.sequence_length - 2, seq_dim=1, batch_dim=0)
            mask_for_label = tf.sign(sliced_targets) # Tensor shape: [batch_size, seq_len]
            self.targets_one_hot = tf.one_hot(sliced_targets, depth=self.num_cls, on_value=1, off_value=0, dtype='int32') # Tensor shape: [batch_size, seq_len, num_cls]
            self.entropy_loss = tf.nn.softmax_cross_entropy_with_logits(logits=self.logits, labels=self.targets_one_hot, name='entropy_loss') # Tensor shape: [batch_size, seq_len]
            self.entropy_loss = tf.multiply(tf.cast(self.entropy_loss, 'float'), tf.cast(mask_for_label, 'float')) # Tensor shape: [batch_size, seq_len]
            self.cost = tf.reduce_mean(self.entropy_loss)

            # predicted label accuracy
            mask_for_label = tf.reshape(mask_for_label, [-1]) # Tensor shape: [batch_size * seq_len, ]
            #self.logits = tf.reshape(self.logits, [-1, self.seq_len, self.num_cls]) # Tensor shape: [batch_size, seq_len, num_cls]
            self.predict_labels = tf.argmax(self.logits, axis=2) # Tensor shape: [batch_size, seq_len]
            predict_match = tf.reshape(tf.equal(self.predict_labels, sliced_targets), [-1])
            predict_match = tf.multiply(mask_for_label, tf.cast(predict_match, 'int64'))
            self.accuracy = tf.reduce_sum(tf.cast(predict_match, 'float')) / tf.reduce_sum(tf.cast(mask_for_label, 'float'))

            # optimizer
            print('MODEL INFO: learning rate = {}'.format(self.lr))
            self.optimizer = tf.train.AdamOptimizer(self.lr).minimize(self.cost)

        # configure model saver
        self.saver = tf.train.Saver(tf.global_variables(), max_to_keep=max_model_to_keep)

        print('MODEL INFO: device is {}'.format(dev_info))
        print('MODEL INFO: BiLstmNetwork model built successfully')

if __name__ == '__main__':
    tf.reset_default_graph()

    BiLstmNetwork(50, 10000, 1000, 256, 64, 10)

