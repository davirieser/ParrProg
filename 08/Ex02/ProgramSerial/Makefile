CC=gcc
CC_FLAGS=-O3 -g3 -lm -pg -fopenmp

EXE_FILE=real

.PHONEY: all
all: real

SRC=$(wildcard *.c)

real: $(SRC)
	$(CC) $(SRC) -o real $(CC_FLAGS)

.PHONEY: clean
clean:
	@rm -f real analysis.txt

analysis.txt: real
	@ ./real
	@ gprof ./real gmon.out >> $@

analysis_fine.txt: real
	@ ./real
	@ gprof --line ./real gmon.out >> $@

.PHONEY: run
run: all
	@echo "Sequential:"
	@./real
	@echo

run_LOCAL_%: real
	@for _ in $(shell seq 1 $*); do \
		./$(EXE_FILE); \
	done

run_LCC_%: real
	@for _ in $(shell seq 1 $*); do \
		sbatch ../../../job.sh $(EXE_FILE); \
	done