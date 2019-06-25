all: benchmark test 

benchmark: dictionary.c dictionary.h a6.c
	gcc -g dictionary.c a6.c -o benchmark

test: a6_test.c dictionary.h dictionary.c
	gcc -g dictionary.c a6_test.c -o test

bits:
	gcc -g a6_bits.c -o bit
	./bit
