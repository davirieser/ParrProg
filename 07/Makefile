
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

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1

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

