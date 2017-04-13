#!/bin/sh
#PBS -q hotel
#PBS -l walltime=20:00:00
file_num=1
train_file="/home/rqiu/spring/data/cifar_5000_subset_data.txt"
test_file="/home/rqiu/spring/data/cifar_5000_test_subset_data.txt"
# Training kernel
/home/rqiu/spring/c/cifar /oasis/tscc/scratch/rqiu/cifar_5000_data_kernel.txt $train_file $train_file 0 5000
# Test kernel
/home/rqiu/spring/c/cifar /oasis/tscc/scratch/rqiu/cifar_5000_test_data_kernel.txt $train_file $test_file 0 500
