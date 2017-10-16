all:
	@gcc compiler_helper.c -c -o compiler_helper.o
	@g++ *.cpp compiler_helper.o -lm -lpng -lexpreval -pthread -O3 -o mandelbrot
	@rm -rf compiler_helper.o
