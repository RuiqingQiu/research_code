CC=g++

all: cifar.cc computeLeKernelRGB.cc computeLeKernelRGB.h 
	$(CC) -o regular cifar.cc computeLeKernelRGB.cc

cifar_max: cifar.cc cifar_max_structure.c
	$(CC) -o cifar_max cifar.cc cifar_max_structure.c

cifar: cifar.cc cifar_structure.c
	$(CC) -o cifar cifar.cc cifar_structure.c

better: better_performance.cc computeLeKernelRGB.cc computeLeKernelRGB.h
	$(CC) -pthread -o better better_performance.cc computeLeKernelRGB.cc

in_memory: load_in_memory.cc computeLeKernelRGB.cc computeLeKernelRGB.h
	$(CC) -o in_memory load_in_memory.cc computeLeKernelRGB.cc

clean: *.sh.*
	rm *.sh.*
