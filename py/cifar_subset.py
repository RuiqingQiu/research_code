# File path for the whole cifar dataset
cifar_whole_data_path = '../../cifar_train_c_2.txt'
cifar_whole_label_path = '../../cifar_train_label.txt'

# A subset of 5000 data points
size = 1000
test_size = 100

# File path for saving the subset
cifar_subset_data_path = '../data/cifar_' + str(size) + '_subset_data.txt'
cifar_subset_label_path = '../data/cifar_' + str(size) + '_subset_label.txt'

cifar_subset_test_data_path = '../data/cifar_' + str(size) + '_test_subset_data.txt'
cifar_subset_test_label_path = '../data/cifar_' + str(size) + '_test_subset_label.txt'

subset_file = open(cifar_subset_data_path, 'w')
test_file = open(cifar_subset_test_data_path, 'w')
count = 0
with open(cifar_whole_data_path) as train_file:
    for line in train_file:
        if count >= size + test_size:
            break
        else:
            if count >= size:
                test_file.write(line)
            else:
                subset_file.write(line)
        count += 1

count = 0

subset_label_file = open(cifar_subset_label_path, 'w')
test_label_file = open(cifar_subset_test_label_path, 'w')
with open(cifar_whole_label_path) as label_file:
    for line in label_file:
        if count >= size + test_size:
            break
        else:
            if count >= size:
                test_label_file.write(line)
            else:
                subset_label_file.write(line)
        count += 1



