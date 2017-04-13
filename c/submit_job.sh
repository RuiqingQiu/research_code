#!/bin/sh
#PBS -q hotel
#PBS -l walltime=10:00:00
file_num=1
start_line=0
end_line=5000
train_file="/oasis/tscc/scratch/rqiu/cifar_train_c.txt"
#/home/rqiu/spring/c/cifar /home/rqiu/spring/data/cifar_5000_data_kernel.txt /home/rqiu/spring/data/cifar_5000_subset_data.txt 5000
# Training kernel
/home/rqiu/spring/c/cifar /oasis/tscc/scratch/rqiu/cifar_$file_num.txt $train_file $train_file $start_line $end_line 

