import numpy as np
from sklearn import svm
import math
import time
import sys
import datetime

train_data = []
train_label = []
test_data = []
test_label = []
training_size = 1000
test_size = 1000

with open("mnist_train_libsvm.txt") as file:
    for line in file:
        data = [0.0] * training_size
        tmp = line.split()
        label = int(tmp[0])
        train_label.append(label)
        for i in xrange(1, len(tmp)):
            (index, value) = tmp[i].split(':')
            index = int(index) - 1
            data[index] = float(value)
        train_data.append(np.array(data))


with open("mnist_test_libsvm.txt") as file:
    for line in file:
        data = [0.0] * training_size
        tmp = line.split()
        label = int(tmp[0])
        test_label.append(label)
        for i in xrange(1, len(tmp)):
            (index, value) = tmp[i].split(':')
            index = int(index) - 1
            data[index] = float(value)
        test_data.append(np.array(data))


clf = svm.SVC(kernel='precomputed')
clf.fit(train_data, train_label)
print "fit done"

predict = clf.predict(test_data)
print predict

error = 0
for i in xrange(len(predict)):
    if predict[i] != test_label[i]:
        error += 1
print clf.kernel
print "Accuracy is " + str(100.0 * (test_size - error) / test_size) + "%"
