import numpy as np
from sklearn import svm
import math
import time
import sys
import datetime


train_file = 'mnist.scale'
test_file = 'mnist.scale.t'

train_data = []
train_label = []
test_data = []
test_label = []
'''
with open(train_file) as file:
    for line in file:
        data = [0.0] * 32 * 32
        tmp = line.split()
        label = int(tmp[0])
        train_label.append(label)
        for i in xrange(1, len(tmp)):
            (index, value) = tmp[i].split(':')
            index = int(index)-1
            i = index / 28 # Row number
            j = index % 28 # Column number
            data[(i+2)*32 + (j+2)] = float(value)
        train_data.append(np.array(data))

np.save('mnist_train.npy', train_data)
np.save('mnist_train_label.npy', train_label)
with open(test_file) as file:
    for line in file:
        data = [0.0] * 32 * 32
        tmp = line.split()
        label = int(tmp[0])
        test_label.append(label)
        for i in xrange(1, len(tmp)):
            (index, value) = tmp[i].split(':')
            index = int(index)-1
            i = index / 28
            j = index % 28
            data[(i+2) * 32 + (j+2)] = float(value)
        test_data.append(np.array(data))
np.save('mnist_test.npy', test_data)
np.save('mnist_test_label.npy', test_label)
'''
start = time.time()
train_data = np.load('mnist_train.npy')
train_label = np.load('mnist_train_label.npy')
test_data = np.load('mnist_test.npy')
test_label = np.load('mnist_test_label.npy')
end = time.time()
print "loading time for all files is " + str(end - start)


#f = open('mnist_test_no_colon.txt', 'w')
#for item in test_data:
#    f.write(" ".join(str(x) for x in item) + "\n")
#exit()
#f = open('mnist_test_label.txt', 'w')
#for item in test_label:
#    f.write(str(item) + "\n")
#exit()


def fastArcCosine(x):
    a0 = 1.5707963050
    a1 = -0.2145988016
    a2 = +0.0889789874
    a3 = -0.0501743046
    a4 = +0.0308918810
    a5 = -0.0170881256
    a6 = +0.0066700901
    a7 = -0.0012624911

    y = math.sqrt(1.0-x)*(a0+(a1+(a2+(a3+(a4+(a5+(a6+a7*x)*x)*x)*x)*x)*x)*x)
    return y

def arcCosKernel(dotProdAB, normAB):
    normAB += sys.float_info.min
    if normAB < dotProdAB:
        normAB = dotProdAB
        #print "probably should not happened"
    cosTh = dotProdAB / normAB
    theta = fastArcCosine(cosTh)
    acosK = (normAB * math.sqrt(math.fabs(1.0-cosTh * cosTh)) + dotProdAB*(math.pi - theta))/math.pi;
    return acosK

def computeLeKernel(A, B):
    #start = time.time()
    A = A.reshape(32, 32)
    B = B.reshape(32, 32)
    n0 = 32
    n1 = 28
    n2 = 14
    n3 = 10
    n4 = 5
    rf = 5
    # Input layer
    k0 = A*B
    u0 = A*A
    v0 = B*B
    #end = time.time()
    #print "0 : " + str(end - start)

    # First Layer
    k1 = np.zeros((n1, n1))
    u1 = np.zeros((n1, n1))
    v1 = np.zeros((n1, n1))
    #end = time.time()
    #print "0.1 : " + str(end - start)
    for i in xrange(n1):
        for j in xrange(n1):
            kk = k0[i:i+rf, j:j+rf].sum()
            uu = u0[i:i+rf, j:j+rf].sum()
            vv = v0[i:i+rf, j:j+rf].sum()
            k1[i][j] = arcCosKernel(kk, math.sqrt(uu*vv))
            u1[i][j] = uu
            v1[i][j] = vv
    #end = time.time()
    #print "1 : " + str(end - start)

    # Second Layer
    k2 = np.zeros((n2,n2))
    u2 = np.zeros((n2,n2))
    v2 = np.zeros((n2,n2))
    for i in xrange(n2):
        for j in xrange(n2):
            x = 2 * i
            y = 2 * j
            k2[i][j] = k1[x][y] + k1[x+1][y] + k1[x][y+1] + k1[x+1][y+1]
            u2[i][j] = u1[x][y] + u1[x+1][y] + u1[x][y+1] + u1[x+1][y+1]
            v2[i][j] = v1[x][y] + v1[x+1][y] + v1[x][y+1] + v1[x+1][y+1]
    #end = time.time()
    #print "2 : " + str(end - start)

    # Third Layer
    k3 = np.zeros((n3, n3))
    u3 = np.zeros((n3, n3))
    v3 = np.zeros((n3, n3))
    for i in xrange(n3):
        for j in xrange(n3):
            kk = k2[i:i+rf, j:j+rf].sum()
            uu = u2[i:i+rf, j:j+rf].sum()
            vv = v2[i:i+rf, j:j+rf].sum()
            k3[i][j] = arcCosKernel(kk,math.sqrt(uu*vv))
            u3[i][j] = uu
            v3[i][j] = vv
    #end = time.time()
    #print "3 : " + str(end - start)

    # Fourth Layer
    k4 = np.zeros((n4,n4))
    u4 = np.zeros((n4,n4))
    v4 = np.zeros((n4,n4))
    for i in xrange(n4):
        for j in xrange(n4):
            x = 2 * i
            y = 2 * j
            k4[i][j] = k3[x][y] + k3[x+1][y] + k3[x][y+1] + k3[x+1][y+1]
            u4[i][j] = u3[x][y] + u3[x+1][y] + u3[x][y+1] + u3[x+1][y+1]
            v4[i][j] = v3[x][y] + v3[x+1][y] + v3[x][y+1] + v3[x+1][y+1]
    #end = time.time()
    #print "4 : " + str(end - start)

    # Fifth Layer
    k5 = u5 = v5 = 0
    for i in xrange(n4):
        for j in xrange(n4):
            k5 += k4[i][j]
            u5 += u4[i][j]
            v5 += v4[i][j]
    normAB = math.sqrt(u5 * v5)
    kAB = arcCosKernel(k5, normAB)
    #end = time.time()
    #print "5 : " + str(end - start)
    return kAB


start = time.time()
a = computeLeKernel(train_data[0], test_data[0])
end = time.time()
print end - start
print a

#for a in train_data[0].reshape(32,32).tolist():
#    print a
for a in test_data[0].reshape(32,32).tolist():
    print a

training_size = 1000 #Number of training data
test_size = 1000 #Number of test data

# Map function for compute custom kernel
def map_func(a):
    print a
    return [computeLeKernel(a,b) for b in train_data[:training_size]]
# Hold all the values for the custom kernel matrix
precomputed = np.zeros((training_size, training_size))
import multiprocessing
pool = multiprocessing.Pool()
#print pool.map(f, range(10))
load_from_file = False
file_name = 'precomputed2017-05-09T12:30:17.483407.npy'


# Compute custom kernel for training
'''
if load_from_file:
    precomputed = np.load(file_name)
else:
    start = time.time()
    #precomputed = map(map_func, train_data[:training_size])
    precomputed = pool.map(map_func, train_data[:training_size])
    end = time.time()
    print end - start
    np.save('precomputed' + datetime.datetime.now().isoformat() + '.npy', precomputed)
'''

#clf = svm.SVC(kernel='linear')
#clf = svm.SVC(kernel='precomputed')
clf = svm.SVC(kernel='rbf')
clf.fit(np.array(train_data)[:training_size], train_label[:training_size])
#clf.fit(precomputed, train_label[:training_size])
print "fit done"

# Compute custom kernel for testing
#precomputed_test = pool.map(map_func, test_data[:test_size])
predict = clf.predict(np.array(test_data[:test_size]))

#predict = clf.predict(precomputed_test)
#print precomputed_test[0]

print test_label[:100]
error = 0
for i in xrange(len(predict)):
    if predict[i] != test_label[i]:
        error += 1
print clf.kernel
print "Accuracy is " + str(100.0 * (test_size - error) / test_size) + "%"
