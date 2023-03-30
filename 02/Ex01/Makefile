CFLAGS = -std=gnu11 -Wall -Wextra -lm

.PHONY: all
all: mandelbrot

.PHONY: clean
clean:
	$(RM) mandelbrot mandelbrot.png

mandelbrot: 01.cpp
	g++ -o mandelbrot 01.cpp

run_lcc2:
	sbatch job.sh

run_lcc2_%: mandelbrot
	for i in $$(seq 1 $*); do\
		$(MAKE) run_lcc2;\
		sleep 0.5;\
	done
