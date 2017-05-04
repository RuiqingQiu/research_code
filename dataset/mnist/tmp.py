import numpy as np
import matplotlib.pyplot as plt
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
        train_data.append(data)
        break
print train_data
plt.imshow(train_data[0])
print train_label






