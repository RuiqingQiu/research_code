CC=g++

all: cifar.cc computeLeKernelRGB.cc computeLeKernelRGB.h 
	$(CC) -o regular cifar.cc computeLeKernelRGB.cc

cifar_max: cifar.cc cifar_max_structure.c
	$(CC) -o cifar_max cifar.cc cifar_max_structure.c

cifar: cifar.cc cifar_structure.c
	$(CC) -o cifar cifar.cc cifar_structure.c

clean: *.sh.*
	rm *.sh.*
