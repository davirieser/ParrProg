
main: main.c
	@ gcc $^ -o $@ -lm -Wall -Werror -Wpedantic

run: main
	@ ./main 5000 1 100 

output.gif: run
	@ gnuplot particle.plt

.PHONY: clean
clean:
	@ rm -f main particle.plt data.dat output.gif

