from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tensorflow.contrib import rnn
from tensorflow.contrib import layers
import numpy as np
from modules import multihead_attention, multihead_attention_combi, feedforward

class Transformer(object):
    def __init__(self, seq_len, input_vocab_size, masked_input_vocab_size, 
                 tag_vocab_size, emb_size, hid_dim, num_cls, 
                 num_heads=4, num_blocks=2, lr=0.001, dev_info='/gpu:0', max_model_to_keep=2):
        self.seq_len = seq_len
        self.input_vocab_size = input_vocab_size
        self.masked_input_vocab_size = masked_input_vocab_size
        self.tag_vocab_size = tag_vocab_size
        self.emb_size = emb_size
        self.hid_size = hid_dim
        self.num_heads = num_heads
        self.num_blocks = num_blocks
        self.num_cls = num_cls
        self.lr = lr
        self.inputs = tf.placeholder(tf.int32, [None, self.seq_len], name='pinyin_inputs')
        self.masked_inputs = tf.placeholder(tf.int32, [None, self.seq_len], name='hanzi_inputs')
        self.input_tags = tf.placeholder(tf.int32, [None, self.seq_len], name='seg_tags')
        self.negs = tf.placeholder(tf.int32, [None, self.seq_len], name='negs')
        self.targets = tf.placeholder(tf.int64, [None, self.seq_len], name='targets')
        self.dropout_keep_prob = tf.placeholder(tf.float32, name='dropout_keep_prob')
        self.sequence_length = tf.reduce_sum(tf.sign(self.targets), axis=1)
        self.sequence_length = tf.cast(self.sequence_length, tf.int32)

        # initialization
        with tf.device(dev_info):
            self.all_output_emb = tf.get_variable(name="all_output_emb", shape=[self.num_cls, self.emb_size], initializer=tf.truncated_normal_initializer(stddev=(1.0 / np.sqrt(self.emb_size))))
            self.all_output_emb = tf.concat((tf.zeros(shape=[1, self.emb_size]),self.all_output_emb[1:, :]), 0)
            self.all_pos_emb = tf.get_variable(name="all_pos_emb", shape=[self.seq_len, self.emb_size], initializer=tf.truncated_normal_initializer(stddev=(1.0 / np.sqrt(self.emb_size))))    

            self.outputs_emb = tf.nn.embedding_lookup(self.all_output_emb, self.targets)
            key_masks = tf.expand_dims(tf.sign(tf.reduce_sum(tf.abs(self.outputs_emb), axis=-1)), -1)
            # Add position embedding
            self.outputs_emb += tf.nn.embedding_lookup(self.all_pos_emb, tf.tile(tf.expand_dims(tf.range(tf.shape(self.targets)[1]), 0), [tf.shape(self.targets)[0], 1]))
            self.outputs_emb *= key_masks

            self.outputs_emb_reversed = tf.reverse_sequence(self.outputs_emb, self.sequence_length, seq_dim=1, batch_dim=0)

        self.sequence_length -= 1

        # network definition
        with tf.device(dev_info):
            self.outputs_emb_sliced = tf.reverse_sequence(self.outputs_emb_reversed[:, 1:, :], self.sequence_length, seq_dim=1, batch_dim=0) # reversed ... <S> -> <S> ...
            params = {"inputs": self.outputs_emb_sliced, "filters": self.emb_size, "kernel_size": 1,
                      "activation": None, "use_bias": True, "kernel_initializer": tf.contrib.layers.xavier_initializer()}
            self.outputs_lm_inputs = tf.layers.conv1d(**params)
            self.outputs_lm_inputs *= key_masks[:, 1:, :]
            
            self.outputs_emb_reversed_sliced = tf.reverse_sequence(self.outputs_emb[:, 1:, :], self.sequence_length, seq_dim=1, batch_dim=0)
            params = {"inputs": self.outputs_emb_reversed_sliced, "filters": self.emb_size, "kernel_size": 1,
                      "activation": None, "use_bias": True, "kernel_initializer": tf.contrib.layers.xavier_initializer()}
            self.outputs_lm_inputs_reversed = tf.layers.conv1d(**params)
            self.outputs_lm_inputs_reversed *= key_masks[:, 1:, :]

            # Fake bi-directional transformer
            def bi_trans_layer(layer_name, layer_inputs, layer_inputs_reversed, hid_size_multiple=1):
                key_masks = tf.sign(tf.reduce_sum(tf.abs(layer_inputs), axis=-1))
                query_masks = key_masks
                with tf.variable_scope(layer_name, reuse=tf.AUTO_REUSE):
                    outputs_fw = layer_inputs
                    outputs_bw = layer_inputs_reversed
                    for i in range(self.num_blocks):
                        with tf.variable_scope("forward_%d"%(i)):
                            outputs_fw = multihead_attention(queries=outputs_fw, 
                                    keys=outputs_fw,
                                    key_masks=key_masks,
                                    query_masks=query_masks,
                                    num_units=hid_size_multiple * self.hid_size, 
                                    num_heads=self.num_heads, 
                                    dropout_rate=1.0-self.dropout_keep_prob,
                                    causality=True, 
                                    scope="forward_self_attention")
                            outputs_fw = feedforward(outputs_fw, num_units=[4*hid_size_multiple * self.hid_size, hid_size_multiple * self.hid_size])

                        with tf.variable_scope("backward_%d"%(i)):
                            outputs_bw = multihead_attention(queries=outputs_bw, 
                                    keys=outputs_bw, 
                                    key_masks=key_masks,
                                    query_masks=query_masks,
                                    num_units=hid_size_multiple * self.hid_size, 
                                    num_heads=self.num_heads, 
                                    dropout_rate=1.0-self.dropout_keep_prob,
                                    causality=True, 
                                    scope="backward_self_attention")
                            outputs_bw = feedforward(outputs_bw, num_units=[4*hid_size_multiple * self.hid_size, hid_size_multiple * self.hid_size])

                    outputs_bw_sliced = tf.reverse_sequence(outputs_bw, self.sequence_length, seq_dim=1, batch_dim=0)[:, 1:, :] # B2, B3, .. Bn+1
                    # [F0, F1, ... Fn-1]
                    outputs_fw_sliced = tf.reverse_sequence(tf.reverse_sequence(outputs_fw, self.sequence_length, seq_dim=1, batch_dim=0)[:, 1:, :], self.sequence_length-1, seq_dim=1, batch_dim=0)

                    # combined_fw_bw = tf.concat([outputs_fw_sliced, outputs_bw_sliced], 2) # [F0+B2, F1+B3, ..., Fn-1 + Bn+1]
                    combined_fw_bw = tf.add(outputs_fw_sliced, outputs_bw_sliced)   #Sum way
                    key_masks = key_masks[:, 1:]
                    query_masks = key_masks
                    for i in range(1):
                        queries = combined_fw_bw
                        with tf.variable_scope("combined_%d"%(i)):
                            combined_outputs = multihead_attention_combi(queries=queries,
                                                forward_keys=outputs_fw_sliced, backward_keys=outputs_bw_sliced,
                                                key_masks=key_masks,
                                                query_masks=query_masks,
                                                num_units=hid_size_multiple * self.hid_size,
                                                num_heads=self.num_heads,
                                                dropout_rate=1.0-self.dropout_keep_prob,
                                                causality=True, 
                                                scope="combine_self_attention")
                            queries = combined_outputs
                return combined_outputs

            self.lm_outputs = bi_trans_layer('bi_trans_layer', self.outputs_lm_inputs, self.outputs_lm_inputs_reversed, 1) # [batch_size, seq_len-2, hid_size]
            params = {"inputs": self.lm_outputs, "filters": self.emb_size, "kernel_size": 1,
                    "activation": None, "use_bias": True, "kernel_initializer": tf.contrib.layers.xavier_initializer()}
            self.lm_outputs = tf.layers.conv1d(**params)
            

            softmax_W = self.all_output_emb
            softmax_b = tf.get_variable('softmax_b', [self.num_cls], initializer=tf.constant_initializer(0.0))
            self.lm_logits = tf.tensordot(self.lm_outputs, softmax_W, axes = [[2], [1]]) + softmax_b

            # max entropy loss
            sliced_targets = self.targets[:, 1:]
            sliced_targets = tf.reverse_sequence(tf.reverse_sequence(sliced_targets, self.sequence_length, seq_dim=1, batch_dim=0)[:, 1:], self.sequence_length-1, seq_dim=1, batch_dim=0)
            mask_for_label = tf.sign(sliced_targets) # Tensor shape: [batch_size, seq_len]
            self.targets_one_hot = tf.one_hot(sliced_targets, depth=self.num_cls, on_value=1, off_value=0, dtype='int32') # Tensor shape: [batch_size, seq_len, num_cls]

            self.lm_loss = tf.nn.softmax_cross_entropy_with_logits(logits=self.lm_logits, labels=self.targets_one_hot, name='entropy_loss') # Tensor shape: [batch_size, seq_len]
            self.lm_loss = tf.multiply(tf.cast(self.lm_loss, 'float'), tf.cast(mask_for_label, 'float')) # Tensor shape: [batch_size, seq_len]
            self.cost = tf.reduce_mean(self.lm_loss)

            # predicted label accuracy
            mask_for_label = tf.reshape(mask_for_label, [-1]) # Tensor shape: [batch_size * seq_len, ]

            self.lm_pred_labels = tf.argmax(self.lm_logits, axis=2) # Tensor shape: [batch_size, seq_len]
            lm_pred_match = tf.reshape(tf.equal(self.lm_pred_labels, sliced_targets), [-1])
            lm_pred_match = tf.multiply(mask_for_label, tf.cast(lm_pred_match, 'int64'))
            self.ave_acc = tf.reduce_sum(tf.cast(lm_pred_match, 'float')) / tf.reduce_sum(tf.cast(mask_for_label, 'float'))
            
            # optimizer
            print('MODEL INFO: learning rate = {}'.format(self.lr))
            self.optimizer = tf.train.AdamOptimizer(self.lr).minimize(self.cost)

        # configure model saver
        self.saver = tf.train.Saver(tf.global_variables(), max_to_keep=max_model_to_keep)

        print('MODEL INFO: device is {}'.format(dev_info))
        print('MODEL INFO: Transformer model built successfully')

if __name__ == '__main__':
    tf.reset_default_graph()

    BiLstmNetwork(50, 10000, 1000, 256, 64, 10)

