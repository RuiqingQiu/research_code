import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm

train_file = 'mnist.scale'
test_file = 'mnist.scale.t'

train_data = []
train_label = []
test_data = []
test_label = []
with open(train_file) as file:
    for line in file:
        data = [0.0] * 32 * 32
        tmp = line.split()
        label = int(tmp[0])
        train_label.append(label)
        for i in xrange(1, len(tmp)):
            (index, value) = tmp[i].split(':')
            index = int(index)
            i = index / 28 # Row number
            j = index % 28 # Column number
            data[(i+2)*32 + (j+2)] = float(value)
        train_data.append(np.array(data))

np.save('mnist_train.npy', train_data)
np.save('mnist_train_label.npy', train_label)
train_data = np.load('mnist_train.npy')
train_label = np.load('mnist_train_label.npy')
#plt.imshow(tmp[0].reshape(32,32), interpolation='None')
#plt.show()

with open(test_file) as file:
    for line in file:
        data = [0.0] * 32 * 32
        tmp = line.split()
        label = int(tmp[0])
        test_label.append(label)
        for i in xrange(1, len(tmp)):
            (index, value) = tmp[i].split(':')
            index = int(index)
            i = index / 28
            j = index % 28
            data[(i+2) * 32 + (j+2)] = float(value)
        test_data.append(np.array(data))
np.save('mnist_test.npy', test_data)
np.save('mnist_test_label.npy', test_label)
test_data = np.load('mnist_test.npy')
test_label = np.load('mnist_test_label.npy')

clf = svm.SVC(kernel='linear')
clf.fit(np.array(train_data), train_label)

predict = clf.predict(np.array(test_data))

error = 0
for i in xrange(len(predict)):
    if predict[i] != test_label[i]:
        error += 1
print error







