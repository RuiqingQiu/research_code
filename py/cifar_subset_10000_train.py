# File path for the whole cifar dataset
cifar_whole_data_path = '/oasis/tscc/scratch/rqiu/cifar_train_c.txt'
cifar_whole_label_path = '../../cifar_train_label.txt'

f_data = open('/oasis/tscc/scratch/rqiu/cifar_0vs1_test_data.txt', 'w')
f_label = open('/oasis/tscc/scratch/rqiu/cifar_0vs1_test_label.txt','w')

label = []
with open(cifar_whole_label_path) as label_file:
    for line in label_file:
        label.append(int(line.split()[0]))

class_count = [0] * 10
count = 0
with open(cifar_whole_data_path) as data_file:
    for line in data_file:
        if count < 20000:
            count += 1
        else:
            if (label[count] == 3 or label[count] == 5) and class_count[label[count]] < 200:
                f_data.write(line)
                f_label.write(str(label[count]) +'\n')
                class_count[label[count]] += 1
            count += 1
        if class_count[3] + class_count[5] == 400:
            break
print class_count

'''
f_data = open('/oasis/tscc/scratch/rqiu/cifar_10000_data.txt','w')
f_label = open('/oasis/tscc/scratch/rqiu/cifar_10000_label.txt','w')
label = []
with open(cifar_whole_label_path) as label_file:
    for line in label_file:
        label.append(int(line.split()[0]))
class_count = [0] * 10
count = 0
with open(cifar_whole_data_path) as data_file:
    for line in data_file:
        if class_count[label[count]] < 1000:
            f_data.write(line)
            f_label.write(str(label[count]) + '\n')
            class_count[label[count]] += 1
        count += 1

print class_count
'''


