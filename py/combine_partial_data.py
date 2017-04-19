
#partial_line_number = [869, 874, 869, 878, 874, 872, 874, 870, 878, 875]
partial_line_number = [875, 875]
print sum(partial_line_number)
f = open('/oasis/tscc/scratch/rqiu/combine_cifar_test_50000.txt', 'w')
for i in xrange(2):
    line_number = partial_line_number[i]
    with open('/oasis/tscc/scratch/rqiu/cifar_test_target_' + str(i) + '.txt') as temp_file:
        count = 1
        for line in temp_file:
            if count < line_number:
                f.write(line)
            else:
                print count
                break
            count += 1

f = open('/oasis/tscc/scratch/rqiu/combine_cifar_test_50000_label.txt', 'w')
for i in xrange(2):
    partial_line_number[i] = partial_line_number[i] + 5000 * i
print partial_line_number

i = 0
with open('/home/rqiu/cifar_test_label.txt') as label_file:
    count = 0
    line_number = partial_line_number[i]
    for line in label_file:
        if count >= 5000 * i and count < line_number - 1:
            f.write(line)
        elif count < 5000 * i:
            x = 1
        else:
            print count
            i += 1
            if i == 2:
                break
            line_number = partial_line_number[i]
        count += 1










