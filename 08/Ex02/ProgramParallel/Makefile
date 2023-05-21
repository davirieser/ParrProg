CC=gcc
CC_FLAGS=-O3 -g3 -lm -pg -fopenmp

EXE_FILE=real
OMP_THREAD_LIST= 1 2 4 8

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
	for _ in $(shell seq 1 $*); do \
		for threads in $(OMP_THREAD_LIST); do \
			export OMP_NUM_THREADS=$$threads && ./$(EXE_FILE); \
		done \
	done

run_LCC_%: real
	@for _ in $(shell seq 1 $*); do \
		for threads in $(OMP_THREAD_LIST); do \
			OMP_NUM_THREADS=$$threads && sbatch ../../job.sh ./$(EXE_FILE); \
		done \
	done