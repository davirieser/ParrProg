
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
	Task1_ENVVAR_SIZE ?= 100
	Task1_ENVVAR_SIZE_VALUES = 100 200 400 600 800 1000 2000 4000 6000 8000 10000
Task1_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task1_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1

# ------ Recipes for Task1 (Ex01/Vectorizing.c) ---------------------------------- #

.PHONY: Task1
Task1:
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		$(foreach Task1_ENVVAR_SIZE, $(Task1_ENVVAR_SIZE_VALUES), \
			export ENVVAR_SIZE=$(Task1_ENVVAR_SIZE) &&  \
			COMPILE_FILE=Ex01/Vectorizing.c \
			EXE_FILE=Ex01/Vectorizing \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task1; \
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

