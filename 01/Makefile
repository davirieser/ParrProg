File1 := slow
File2 := medium
File3 := fast

STD := -std=c11
LINK := -fopenmp

THREAD_NUMBERS := 1 2 3 4 5 6 7 8 9 10 11 12
COMPILE_STD=-O0
OPTIMISATION_LEVELS := 0 1 2 3 s fast


all: $(File1) $(File2) $(File3) 

.PHONY: all

%: %.c
	gcc $^ -o $@ $(STD) $(LINK) $(COMPILE_STD)


runall: $(patsubst %,runall-%,$(OPTIMISATION_LEVELS))
runall-%:
	touch $(File1).c $(File2).c $(File3).c ; \
	$(MAKE) COMPILE_STD=-O$*    all $(File1) run1all $(File2) run2all $(File3) run3all ; \


run1all: $(patsubst %,run1all-%,$(THREAD_NUMBERS))
run1all-%:
	OMP_NUM_THREADS=$* ./$(File1)


run2all: $(patsubst %,run2all-%,$(THREAD_NUMBERS))
run2all-%:
	OMP_NUM_THREADS=$* ./$(File2)


run3all: $(patsubst %,run3all-%,$(THREAD_NUMBERS))
run3all-%:
	OMP_NUM_THREADS=$* ./$(File3)

runlcc2: run1_1 run1_8 run2_1 run2_8 run3_1 run3_8

run1_1:
	touch $(File1).c
	$(MAKE) COMPILE_STD=-Ofast 
	OMP_NUM_THREADS=1 ./$(File1)

run1_8:
	touch $(File1).c
	$(MAKE) COMPILE_STD=-O0 
	OMP_NUM_THREADS=8 ./$(File1)

run2_1:
	touch $(File2).c
	$(MAKE) COMPILE_STD=-O1 
	OMP_NUM_THREADS=1 ./$(File2)

run2_8:
	touch $(File2).c
	$(MAKE) COMPILE_STD=-O3
	OMP_NUM_THREADS=8 ./$(File2)

run3_1:
	touch $(File3).c
	$(MAKE) COMPILE_STD=-O2 
	OMP_NUM_THREADS=1 ./$(File3)

run3_8:
	touch $(File3).c
	$(MAKE) COMPILE_STD=-Os
	OMP_NUM_THREADS=8 ./$(File3)