
# Example usage: python libsvm_format.py ../data/cifar_5000_data_kernel.txt ../data/cifar_5000_subset_label.txt ../data/cifar_5000_libsvm_kernel.txt

import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)


data_file_path = sys.argv[1]
label_file_path = sys.argv[2]

'''
f = open(sys.argv[3], 'w')

label = []
with open(label_file_path) as label_file:
    for line in label_file:
        label.append(line.split()[0])

count = 0
with open(data_file_path) as file:
    for line in file:
        f.write(label[count] + " " + "0:" + str(count+1) + " ")
        tmp = line.split()
        index = 1
        for item in tmp:
            f.write(str(index)+":"+item+" ")
            index += 1
        f.write("\n")
        count += 1
'''
