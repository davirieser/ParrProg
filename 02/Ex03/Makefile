CFLAGS = -Wall -Wextra -lm -pthread -Wpedantic
COMPILES_STD = -std=c++11

File = monte_carlo
File_ext = cpp

VARIANTS := 1 2
THREAD_NUMBERS_PC := 1 2 4 8 12
THREAD_NUMBERS_LCC2 := 1 8

.PHONY: all
all: $(File)

.PHONY: clean
clean:
	$(RM) $(File)

$(File): $(File).$(File_ext)
	g++ -o $(File) $(CFLAGS) $(File).$(File_ext) $(COMPILES_STD)

run_all-%:
	for i in $$(seq 1 $*); do\
		$(MAKE) run_all; \
	done

run_all:
	./$(File) 0
	$(MAKE) $(patsubst %,runall_var-%,$(THREAD_NUMBERS_PC))

runall_var-%:
	export JOBVAR_VARIANT=$* ; \
	$(MAKE) $(patsubst %,runall_var_th-%,$(THREAD_NUMBERS_PC))

runall_var_th-%:
	./$(File) $(JOBVAR_VARIANT) $*

run_lcc2_all-%:
	for i in $$(seq 1 $*); do \
		$(MAKE) run_lcc2_all ; \
	done

run_lcc2_all: $(File)
	export JOBVAR_VARIANT=0 ; \
	runall_lcc2-th-1
	$(MAKE) $(patsubst %,runall_lcc2-%,$(VARIANTS))

runall_lcc2-%:
	export JOBVAR_VARIANT=$* ; \
	$(MAKE) $(patsubst %,runall_lcc2-th-%,$(THREAD_NUMBERS_LCC2))

runall_lcc2-th-%:
	export JOBVAR_THREADS=$* ; \
	sbatch job.sh ; \
	sleep 1

