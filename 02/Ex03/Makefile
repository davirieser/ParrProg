CFLAGS = -Wall -Wextra -lm -pthread -Wpedantic
COMPILES_STD = -std=c++11

File = monte_carlo
File_ext = cpp

VARIANTS := 1 2
THREAD_NUMBERS_PC := 1 2 4 8 12

.PHONY: all
all: $(File)

.PHONY: clean
clean:
	$(RM) $(File)

$(File): $(File).$(File_ext)
	g++ -o $(File) $(CFLAGS) $(File).$(File_ext) $(COMPILES_STD)

run_all_20times:
	for i in $$(seq 1 100); do \
		$(MAKE) run_all; \
	done

run_all:
	./$(File) 0
	$(MAKE) $(patsubst %,runall1-%,$(THREAD_NUMBERS_PC))
	$(MAKE) $(patsubst %,runall2-%,$(THREAD_NUMBERS_PC))

runall1-%:
	./$(File) 1 $*
runall2-%:
	./$(File) 2 $*

run_lcc2_all:
	$(MAKE) $(patsubst %,runall_lcc2-%,$(VARIANTS))

runall_lcc2-%:
	export JOBVAR_VARIANT=$*
	$(MAKE) $(patsubst %,runall_lcc2-th-%,$(THREAD_NUMBERS_PC))

runall_lcc2-th-%:
	export JOBVAR_THREADS=$*
	sbatch job.sh

