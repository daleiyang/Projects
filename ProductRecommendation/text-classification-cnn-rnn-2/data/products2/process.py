#coding=utf-8

import sys
import numpy as np
from random import shuffle

reload(sys)
sys.setdefaultencoding('utf8')

f = open("original", "r")
lines = f.readlines()
shuffle(lines)
#indices = np.random.permutation(np.arange(len(lines)))

num = len(lines)
trainN = int(num * 1.0 * 0.75)
testN = int(num * 1.0 * 0.15) + trainN
valN = max(int(num * 1.0 * 0.10) + testN, num)

#print(trainN)
#print(testN)
#print(valN)

train = []
test = []
val = []

train = lines[0:trainN]
test = lines[trainN:testN]
val = lines[testN:num]

f = open("train.txt", "w")
for line in train:
    f.write(line)

f = open("test.txt", "w")
for line in test:
    f.write(line)

f = open("val.txt", "w")
for line in val:
    f.write(line)

