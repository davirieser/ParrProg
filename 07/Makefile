
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LOCAL
# ------ Configuration Values for Task1_1 (Ex01/Vectorizing.c) ---------------------------------- #

Task1_1_FILE := Ex01/Vectorizing.c
Task1_1_EXE := Ex01/Vectorizing.c
Task1_1_LINKS := -fopenmp
Task1_1_OPTIMIZATION_LEVEL ?= -O1
Task1_1_OPTIMIZATION_LEVELS := -O1
Task1_1_COMPILER ?= gcc
	Task1_1_ENVVAR_SIZE ?= 128
	Task1_1_ENVVAR_SIZE_VALUES = 128 256 512 1024 2048 4096 8192 16384
	Task1_1_ENVVAR_VECTORIZED ?= 0
	Task1_1_ENVVAR_VECTORIZED_VALUES = 0
Task1_1_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task1_1_LINKS)

# ------ Configuration Values for Task1_2 (Ex01/Vectorizing.c) ---------------------------------- #

Task1_2_FILE := Ex01/Vectorizing.c
Task1_2_EXE := Ex01/Vectorizing.c
Task1_2_LINKS := -fopenmp -msse2 -ffast-math -fassociative-math -ftree-vectorize
Task1_2_OPTIMIZATION_LEVEL ?= -O1
Task1_2_OPTIMIZATION_LEVELS := -O1
Task1_2_COMPILER ?= gcc
	Task1_2_ENVVAR_SIZE ?= 128
	Task1_2_ENVVAR_SIZE_VALUES = 128 256 512 1024 2048 4096 8192 16384
	Task1_2_ENVVAR_VECTORIZED ?= 1
	Task1_2_ENVVAR_VECTORIZED_VALUES = 1
Task1_2_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task1_2_LINKS)

# ------ Configuration Values for Task2 (Ex02/Vectorizing.c) ---------------------------------- #

Task2_FILE := Ex02/Vectorizing.c
Task2_EXE := Ex02/Vectorizing.c
Task2_LINKS := -fopenmp
Task2_OPTIMIZATION_LEVEL ?= -O1
Task2_OPTIMIZATION_LEVELS := -O1
Task2_COMPILER ?= gcc
	Task2_ENVVAR_SIZE ?= 128
	Task2_ENVVAR_SIZE_VALUES = 128 256 512 1024 2048 4096 8192 16384
Task2_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_LINKS)

# ------ Configuration Values for Task3 (Ex03/Vectorizing.c) ---------------------------------- #

Task3_FILE := Ex03/Vectorizing.c
Task3_EXE := Ex03/Vectorizing.c
Task3_LINKS := -fopenmp
Task3_OPTIMIZATION_LEVEL ?= -O1
Task3_OPTIMIZATION_LEVELS := -O1
Task3_COMPILER ?= gcc
	Task3_ENVVAR_SIZE ?= 128
	Task3_ENVVAR_SIZE_VALUES = 128 256 512 1024 2048 4096 8192 16384
Task3_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task3_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1_1 Task1_2 Task2 Task3

# ------ Recipes for Task1_1 (Ex01/Vectorizing.c) ---------------------------------- #

.PHONY: Task1_1
Task1_1:
	@ $(foreach opt,$(Task1_1_OPTIMIZATION_LEVELS), \
		$(foreach Task1_1_ENVVAR_SIZE, $(Task1_1_ENVVAR_SIZE_VALUES), \
			$(foreach Task1_1_ENVVAR_VECTORIZED, $(Task1_1_ENVVAR_VECTORIZED_VALUES), \
				export ENVVAR_SIZE=$(Task1_1_ENVVAR_SIZE) &&  \
				export ENVVAR_VECTORIZED=$(Task1_1_ENVVAR_VECTORIZED) &&  \
				COMPILE_FILE=Ex01/Vectorizing.c \
				EXE_FILE=Ex01/Vectorizing \
				OPTIMIZATION_LEVEL=$(opt) \
				$(MAKE) -e --no-print-directory run_Task1_1; \
			) \
		) \
	)

.PHONY: Task1_1_OPT
Task1_1_OPT: 
	@ $(foreach opt,$(Task1_1_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_1_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task1_1; \
	)

.PHONY: Task1_1_ENVVAR_SIZE
Task1_1_ENVVAR_SIZE:
	$(foreach val,$(Task1_1_ENVVAR_SIZE_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_1_ENVVAR_SIZE=$(val) \
		$(MAKE) -e --no-print-directory run_Task1_1;\
	)

.PHONY: Task1_1_ENVVAR_VECTORIZED
Task1_1_ENVVAR_VECTORIZED:
	$(foreach val,$(Task1_1_ENVVAR_VECTORIZED_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_1_ENVVAR_VECTORIZED=$(val) \
		$(MAKE) -e --no-print-directory run_Task1_1;\
	)

.PHONY: run_Task1_1
run_Task1_1:
	$(MAKE) -e --no-print-directory compile_Task1_1
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task1_1
compile_Task1_1:
	$(Task1_1_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task1_1_CFLAGS)  $(Task1_1_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task1_2 (Ex01/Vectorizing.c) ---------------------------------- #

.PHONY: Task1_2
Task1_2:
	@ $(foreach opt,$(Task1_2_OPTIMIZATION_LEVELS), \
		$(foreach Task1_2_ENVVAR_SIZE, $(Task1_2_ENVVAR_SIZE_VALUES), \
			$(foreach Task1_2_ENVVAR_VECTORIZED, $(Task1_2_ENVVAR_VECTORIZED_VALUES), \
				export ENVVAR_SIZE=$(Task1_2_ENVVAR_SIZE) &&  \
				export ENVVAR_VECTORIZED=$(Task1_2_ENVVAR_VECTORIZED) &&  \
				COMPILE_FILE=Ex01/Vectorizing.c \
				EXE_FILE=Ex01/Vectorizing \
				OPTIMIZATION_LEVEL=$(opt) \
				$(MAKE) -e --no-print-directory run_Task1_2; \
			) \
		) \
	)

.PHONY: Task1_2_OPT
Task1_2_OPT: 
	@ $(foreach opt,$(Task1_2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_2_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task1_2; \
	)

.PHONY: Task1_2_ENVVAR_SIZE
Task1_2_ENVVAR_SIZE:
	$(foreach val,$(Task1_2_ENVVAR_SIZE_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_2_ENVVAR_SIZE=$(val) \
		$(MAKE) -e --no-print-directory run_Task1_2;\
	)

.PHONY: Task1_2_ENVVAR_VECTORIZED
Task1_2_ENVVAR_VECTORIZED:
	$(foreach val,$(Task1_2_ENVVAR_VECTORIZED_VALUES), \
		COMPILE_FILE=Ex01/Vectorizing.c \
		EXE_FILE=Ex01/Vectorizing \
		Task1_2_ENVVAR_VECTORIZED=$(val) \
		$(MAKE) -e --no-print-directory run_Task1_2;\
	)

.PHONY: run_Task1_2
run_Task1_2:
	$(MAKE) -e --no-print-directory compile_Task1_2
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task1_2
compile_Task1_2:
	$(Task1_2_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task1_2_CFLAGS)  $(Task1_2_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task2 (Ex02/Vectorizing.c) ---------------------------------- #

.PHONY: Task2
Task2:
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		$(foreach Task2_ENVVAR_SIZE, $(Task2_ENVVAR_SIZE_VALUES), \
			export ENVVAR_SIZE=$(Task2_ENVVAR_SIZE) &&  \
			COMPILE_FILE=Ex02/Vectorizing.c \
			EXE_FILE=Ex02/Vectorizing \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task2; \
		) \
	)

.PHONY: Task2_OPT
Task2_OPT: 
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/Vectorizing.c \
		EXE_FILE=Ex02/Vectorizing \
		Task2_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2; \
	)

.PHONY: Task2_ENVVAR_SIZE
Task2_ENVVAR_SIZE:
	$(foreach val,$(Task2_ENVVAR_SIZE_VALUES), \
		COMPILE_FILE=Ex02/Vectorizing.c \
		EXE_FILE=Ex02/Vectorizing \
		Task2_ENVVAR_SIZE=$(val) \
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


# ------ Recipes for Task3 (Ex03/Vectorizing.c) ---------------------------------- #

.PHONY: Task3
Task3:
	@ $(foreach opt,$(Task3_OPTIMIZATION_LEVELS), \
		$(foreach Task3_ENVVAR_SIZE, $(Task3_ENVVAR_SIZE_VALUES), \
			export ENVVAR_SIZE=$(Task3_ENVVAR_SIZE) &&  \
			COMPILE_FILE=Ex03/Vectorizing.c \
			EXE_FILE=Ex03/Vectorizing \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task3; \
		) \
	)

.PHONY: Task3_OPT
Task3_OPT: 
	@ $(foreach opt,$(Task3_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex03/Vectorizing.c \
		EXE_FILE=Ex03/Vectorizing \
		Task3_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task3; \
	)

.PHONY: Task3_ENVVAR_SIZE
Task3_ENVVAR_SIZE:
	$(foreach val,$(Task3_ENVVAR_SIZE_VALUES), \
		COMPILE_FILE=Ex03/Vectorizing.c \
		EXE_FILE=Ex03/Vectorizing \
		Task3_ENVVAR_SIZE=$(val) \
		$(MAKE) -e --no-print-directory run_Task3;\
	)

.PHONY: run_Task3
run_Task3:
	$(MAKE) -e --no-print-directory compile_Task3
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task3
compile_Task3:
	$(Task3_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task3_CFLAGS)  $(Task3_OPTIMIZATION_LEVEL) 

