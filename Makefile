
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LOCAL
# ------ Configuration Values for Task1 (Ex01/01.cpp) ---------------------------------- #

Task1_FILE := Ex01/01.cpp
Task1_EXE := Ex01/01.cpp
Task1_LINKS := -fopenmp
Task1_OPTIMIZATION_LEVEL ?= -O0
Task1_OPTIMIZATION_LEVELS := -O0 -O3 -Ofast
Task1_COMPILER ?= g++
# Compiler Flags
	Task1_VARIANT ?= 0
	Task1_VARIANT_VALUES = 0 1
# Environment Variables
	Task1_OMP_NUM_THREADS ?= 1
	Task1_OMP_NUM_THREADS_VALUES = 1 2 4 8
Task1_CFLAGS=$(STD_CFLAGS)  $(Task1_LINKS)

# ------ Configuration Values for Task2 (Ex02/main.cpp) ---------------------------------- #

Task2_FILE := Ex02/main.cpp
Task2_EXE := Ex02/main.cpp
Task2_LINKS := 
Task2_OPTIMIZATION_LEVEL ?= -O0
Task2_OPTIMIZATION_LEVELS := -O0 -O3 -Ofast
Task2_COMPILER ?= g++
Task2_CFLAGS=$(STD_CFLAGS)  $(Task2_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1 Task2

# ------ Recipes for Task1 (Ex01/01.cpp) ---------------------------------- #

.PHONY: Task1
Task1:
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		$(foreach Task1_OMP_NUM_THREADS, $(Task1_OMP_NUM_THREADS_VALUES), \
			$(foreach VARIANT, $(Task1_VARIANT_VALUES), \
				export OMP_NUM_THREADS=$(Task1_OMP_NUM_THREADS) &&  \
				Task1_VARIANT=$(VARIANT) \
				COMPILE_FILE=Ex01/01.cpp \
				EXE_FILE=Ex01/01 \
				OPTIMIZATION_LEVEL=$(opt) \
				$(MAKE) -e --no-print-directory run_Task1; \
			) \
		) \
	)

.PHONY: Task1_OPT
Task1_OPT: 
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex01/01.cpp \
		EXE_FILE=Ex01/01 \
		Task1_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task1; \
	)

.PHONY: Task1_VARIANT
Task1_VARIANT: 
	$(foreach val,$(Task1_VARIANT_VALUES), \
		COMPILE_FILE=Ex01/01.cpp \
		EXE_FILE=Ex01/01 \
		Task1_VARIANT=$(val) \
		$(MAKE) -e --no-print-directory run_Task1; \
	)

.PHONY: Task1_OMP_NUM_THREADS
Task1_OMP_NUM_THREADS:
	$(foreach val,$(Task1_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex01/01.cpp \
		EXE_FILE=Ex01/01 \
		Task1_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: run_Task1
run_Task1:
	$(MAKE) -e --no-print-directory compile_Task1
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task1
compile_Task1:
	$(Task1_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task1_CFLAGS)  $(Task1_OPTIMIZATION_LEVEL) -DVARIANT=$(Task1_VARIANT)


# ------ Recipes for Task2 (Ex02/main.cpp) ---------------------------------- #

.PHONY: Task2
Task2:
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/main.cpp \
		EXE_FILE=Ex02/main \
		OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2; \
	)

.PHONY: Task2_OPT
Task2_OPT: 
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/main.cpp \
		EXE_FILE=Ex02/main \
		Task2_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2; \
	)

.PHONY: run_Task2
run_Task2:
	$(MAKE) -e --no-print-directory compile_Task2
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task2
compile_Task2:
	$(Task2_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_CFLAGS)  $(Task2_OPTIMIZATION_LEVEL) 

