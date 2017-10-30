all:
	@gcc compiler_helper.c -c -o compiler_helper.o
	@g++ *.cpp compiler_helper.o -lm -lpng -lexpreval -pthread -O3 -lgmp -lquadmath -o mandelbrot
	@rm -rf compiler_helper.o
