
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LCC
# ------ Configuration Values for Task1 (Ex01/Vectorizing.c) ---------------------------------- #

Task1_FILE := Ex01/Vectorizing.c
Task1_EXE := Ex01/Vectorizing.c
Task1_LINKS := -fopenmp
Task1_OPTIMIZATION_LEVEL ?= -O1
Task1_OPTIMIZATION_LEVELS := -O1
Task1_COMPILER ?= gcc
	Task1_ENVVAR_SIZE ?= 128
	Task1_ENVVAR_SIZE_VALUES = 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072
	Task1_ENVVAR_VECTORIZED ?= 0
	Task1_ENVVAR_VECTORIZED_VALUES = 0
Task1_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task1_LINKS)

# ------ Configuration Values for Task2 (Ex01/Vectorizing.c) ---------------------------------- #

Task2_FILE := Ex01/Vectorizing.c
Task2_EXE := Ex01/Vectorizing.c
Task2_LINKS := -fopenmp -msse2 -ffast-math -fassociative-math -ftree-vectorize
Task2_OPTIMIZATION_LEVEL ?= -O1
Task2_OPTIMIZATION_LEVELS := -O1
Task2_COMPILER ?= gcc
	Task2_ENVVAR_SIZE ?= 128
	Task2_ENVVAR_SIZE_VALUES = 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072
	Task2_ENVVAR_VECTORIZED ?= 1
	Task2_ENVVAR_VECTORIZED_VALUES = 1
Task2_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1 Task2

# ------ Recipes for Task1 (Ex01/Vectorizing.c) ---------------------------------- #

.PHONY: Task1
Task1:
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		$(foreach Task1_ENVVAR_SIZE, $(Task1_ENVVAR_SIZE_VALUES), \
			$(foreach Task1_ENVVAR_VECTORIZED, $(Task1_ENVVAR_VECTORIZED_VALUES), \
				export ENVVAR_SIZE=$(Task1_ENVVAR_SIZE) &&  \
				export ENVVAR_VECTORIZED=$(Task1_ENVVAR_VECTORIZED) &&  \
				COMPILE_FILE=Ex01/Vectorizing.c \
				EXE_FILE=Ex01/Vectorizing \
				OPTIMIZATION_LEVEL=$(opt) \
				$(MAKE) -e --no-print-directory run_Task1; \
			) \
		) \
	)

.PHONY: Task1_OPT
Task1_OPT: 
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task1; \
	)

.PHONY: Task1_ENVVAR_SIZE
Task1_ENVVAR_SIZE:
	$(foreach val,$(Task1_ENVVAR_SIZE_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_ENVVAR_SIZE=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_ENVVAR_VECTORIZED
Task1_ENVVAR_VECTORIZED:
	$(foreach val,$(Task1_ENVVAR_VECTORIZED_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_ENVVAR_VECTORIZED=$(val) \
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
	$(Task1_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task1_CFLAGS)  $(Task1_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task2 (Ex01/Vectorizing.c) ---------------------------------- #

.PHONY: Task2
Task2:
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		$(foreach Task2_ENVVAR_SIZE, $(Task2_ENVVAR_SIZE_VALUES), \
			$(foreach Task2_ENVVAR_VECTORIZED, $(Task2_ENVVAR_VECTORIZED_VALUES), \
				export ENVVAR_SIZE=$(Task2_ENVVAR_SIZE) &&  \
				export ENVVAR_VECTORIZED=$(Task2_ENVVAR_VECTORIZED) &&  \
				COMPILE_FILE=Ex01/Vectorizing.c \
				EXE_FILE=Ex01/Vectorizing \
				OPTIMIZATION_LEVEL=$(opt) \
				$(MAKE) -e --no-print-directory run_Task2; \
			) \
		) \
	)

.PHONY: Task2_OPT
Task2_OPT: 
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task2_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2; \
	)

.PHONY: Task2_ENVVAR_SIZE
Task2_ENVVAR_SIZE:
	$(foreach val,$(Task2_ENVVAR_SIZE_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task2_ENVVAR_SIZE=$(val) \
		$(MAKE) -e --no-print-directory run_Task2;\
	)

.PHONY: Task2_ENVVAR_VECTORIZED
Task2_ENVVAR_VECTORIZED:
	$(foreach val,$(Task2_ENVVAR_VECTORIZED_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task2_ENVVAR_VECTORIZED=$(val) \
		$(MAKE) -e --no-print-directory run_Task2;\
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

