#!/bin/sh
#PBS -q hotel
#PBS -l walltime=20:00:00
file_num=1
executable="/home/rqiu/spring/c/cifar"
train_file="/oasis/tscc/scratch/rqiu/cifar_train_c.txt"
test_file="/oasis/tscc/scratch/rqiu/cifar_test_c.txt"
start_line=0
end_line=5000
target_file="/oasis/tscc/scratch/rqiu/cifar_target_1.txt"
# Training kernel
$executable $target_file $train_file $train_file $start_line $end_line
# Test kernel
$executable $target_file $test_file $train_file $start_line $end_line

