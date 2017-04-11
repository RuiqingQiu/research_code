#!/bin/sh
#PBS -q hotel
#PBS -l walltime=10:00:00
/home/rqiu/spring/c/cifar /home/rqiu/spring/data/cifar_5000_data_kernel.txt /home/rqiu/spring/data/cifar_5000_subset_data.txt 5000
