#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from numpy.random import choice
from random import random
import re
from collections import Counter, defaultdict
import sys
import pickle
import time

def random_replace_sent(input_file, output_file, char_prob, replace_rate=0.15):
    #input file need the first column is pinyin and last column is chars
    out_f = open(output_file, 'w')
    for line in open(input_file, 'r'):
        columns = line.strip().split('\t')
        pinyins = columns[0].split()
        chars = columns[-1].split()
        replaced_chars = []
        assert (len(pinyins)==len(chars))
        for i in range(len(chars)):
            if re.match("^[\u4e00-\u9fa5]$", chars[i]):
                if random() < replace_rate:
                    keys = list(char_prob[pinyins[i]].keys())
                    values = list(char_prob[pinyins[i]].values())
                    new_char = choice(keys, 1, p=values)[0]
                    count = 0
                    while new_char == chars[i] and len(char_prob[pinyins[i]])>=2 and count <= 10:
                        new_char = choice(keys, 1, p=values)[0]
                        count += 1
                    # print("Replace %s to %s"%(chars[i], new_char))
                    replaced_chars.append(new_char)
                else:
                    replaced_chars.append(chars[i])
            else:
                replaced_chars.append(chars[i])
        out_f.write(' '.join(replaced_chars)+ '\n')
    out_f.close()

if __name__ == "__main__":
    if not len(sys.argv) == 4:
        print("usage: python %s input_file pickle_dict output_file")% (sys.argv[0])
        sys.exit(1)

    start_time = time.time()
    char_prob = pickle.load(open(sys.argv[2], 'rb'))
    random_replace_sent(sys.argv[1], sys.argv[3], char_prob)

    #dump the pinyin char freq dict

    print("preprocess cost time: ", time.time() - start_time)
