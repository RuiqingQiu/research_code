#!/bin/sh
#PBS -q hotel
#PBS -l walltime=10:00:00
#/home/rqiu/spring/c/cifar /home/rqiu/spring/data/cifar_5000_data_kernel.txt /home/rqiu/spring/data/cifar_5000_subset_data.txt 5000
# Training kernel
/home/rqiu/spring/c/cifar /home/rqiu/spring/data/cifar_1000_data_kernel_cifar_structure.txt /home/rqiu/spring/data/cifar_1000_subset_data.txt /home/rqiu/spring/data/cifar_1000_subset_data.txt 1000
# Test kernel
/home/rqiu/spring/c/cifar /home/rqiu/spring/data/cifar_1000_test_data_kernel_cifar_structure.txt /home/rqiu/spring/data/cifar_1000_test_subset_data.txt /home/rqiu/spring/data/cifar_1000_subset_data.txt 1000

