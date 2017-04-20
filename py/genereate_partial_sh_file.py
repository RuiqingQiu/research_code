data_size = 10000
start = 0
increment_size = 2500
partition = data_size / increment_size
start = 0
test = True
for p in xrange(partition):
    f = open('partial_test_'+str(p)+'.sh', 'w')
    with open('../c/compute_partial_kernel.sh') as template_file:
        count = 0
        for line in template_file:
            if count == 7:
                f.write("start_line=" + str(start + (p * increment_size)) + "\n")
            elif count == 8:
                f.write("end_line=" + str(start + ((p+1) * increment_size)) + "\n")
            elif count == 9:
                f.write("target_file=\"/oasis/tscc/scratch/rqiu/cifar_test_target_"+str(p)+".txt\"\n")
            elif count == 11:
                if not test:
                    f.write(line)
            elif count == 13:
                if test:
                    f.write(line)
            else:
                f.write(line)
            count += 1



