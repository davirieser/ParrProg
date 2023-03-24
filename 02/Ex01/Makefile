CFLAGS = -std=gnu11 -Wall -Wextra -lm

.PHONY: all
all: mandelbrot

.PHONY: clean
clean:
	$(RM) mandelbrot mandelbrot.png

mandelbrot: 01.cpp
	g++ -o mandelbrot 01.cpp
