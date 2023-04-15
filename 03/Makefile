
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LCC
# ------ Configuration Values for Task1 (Ex01/monte_carlo.cpp) ---------------------------------- #

Task1_FILE := Ex01/monte_carlo.cpp
Task1_EXE := Ex01/monte_carlo.cpp
Task1_LINKS := -fopenmp
Task1_OPTIMIZATION_LEVEL ?= -Ofast
Task1_OPTIMIZATION_LEVELS := -Ofast
Task1_PROFILER ?= 
Task1_COMPILER ?= g++
# Compiler Flags
	Task1_VARIANT ?= 0
	Task1_VARIANT_VALUES = 0 1 2
# Environment Variables
	Task1_OMP_NUM_THREADS ?= 1
	Task1_OMP_NUM_THREADS_VALUES = 1 2 4 8
Task1_CFLAGS=$(STD_CFLAGS)  $(Task1_LINKS)

# ------ Configuration Values for Task2 (Ex02/modifiedCode.c) ---------------------------------- #

Task2_FILE := Ex02/modifiedCode.c
Task2_EXE := Ex02/modifiedCode.c
Task2_LINKS := -fopenmp
Task2_OPTIMIZATION_LEVEL ?= -Ofast
Task2_OPTIMIZATION_LEVELS := -Ofast
Task2_PROFILER ?= 
Task2_COMPILER ?= gcc
# Compiler Flags
	Task2_NUM_SAMPLES ?= 1500
	Task2_NUM_SAMPLES_VALUES = 1500
# Environment Variables
	Task2_OMP_NUM_THREADS ?= 1
	Task2_OMP_NUM_THREADS_VALUES = 1 2 4 8
Task2_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_LINKS)

# ------ Configuration Values for Task4 (Ex02/originalCode.c) ---------------------------------- #

Task4_FILE := Ex02/originalCode.c
Task4_EXE := Ex02/originalCode.c
Task4_LINKS := -fopenmp
Task4_OPTIMIZATION_LEVEL ?= -Ofast
Task4_OPTIMIZATION_LEVELS := -Ofast
Task4_PROFILER ?= 
Task4_COMPILER ?= gcc
# Compiler Flags
	Task4_NUM_SAMPLES ?= 1500
	Task4_NUM_SAMPLES_VALUES = 1500
# Environment Variables
	Task4_OMP_NUM_THREADS ?= 1
	Task4_OMP_NUM_THREADS_VALUES = 1 2 4 8
Task4_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task4_LINKS)

# ------ Configuration Values for Task3 (Ex03/monte_carlo.cpp) ---------------------------------- #

Task3_FILE := Ex03/monte_carlo.cpp
Task3_EXE := Ex03/monte_carlo.cpp
Task3_LINKS := -fopenmp
Task3_OPTIMIZATION_LEVEL ?= -Ofast
Task3_OPTIMIZATION_LEVELS := -Ofast
Task3_PROFILER ?= 
Task3_COMPILER ?= g++
# Compiler Flags
	Task3_VARIANT ?= 0
	Task3_VARIANT_VALUES = 0 1 2
# Environment Variables
	Task3_OMP_NUM_THREADS ?= 8
	Task3_OMP_NUM_THREADS_VALUES = 8
Task3_CFLAGS=$(STD_CFLAGS)  $(Task3_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1 Task2 Task4 Task3

# ------ Recipes for Task1 (Ex01/monte_carlo.cpp) ---------------------------------- #

.PHONY: Task1
Task1:
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		$(foreach OMP_NUM_THREADS, $(Task1_OMP_NUM_THREADS_VALUES), \
			$(foreach VARIANT, $(Task1_VARIANT_VALUES), \
				export EXECUTION_VARIANT=$(VARIANT) && \
				Task1_OPTIMIZATION_LEVEL=$(opt) \
				COMPILE_FILE=Ex01/monte_carlo.cpp \
				EXE_FILE=Ex01/monte_carlo \
				Task1_OMP_NUM_THREADS=$(OMP_NUM_THREADS) \
				Task1_VARIANT=$(VARIANT) \
				$(MAKE) -e --no-print-directory run_Task1; \
				sleep 1; \
			) \
		) \
	)

.PHONY: run_Task1
run_Task1:
	$(MAKE) -e --no-print-directory compile_Task1
ifneq ($(TARGET),LCC)
	OMP_NUM_THREADS=$(Task1_OMP_NUM_THREADS)
	$(Task1_PROFILER) ./$(EXE_FILE)
else
	OMP_NUM_THREADS=$(Task1_OMP_NUM_THREADS)
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: Task1_OPT
Task1_OPT: 
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex01/monte_carlo.cpp \
		EXE_FILE=Ex01/monte_carlo \
		Task1_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task1; \
	)

.PHONY: Task1_VARIANT
Task1_VARIANT: 
	$(foreach val,$(Task1_VARIANT_VALUES), \
		COMPILE_FILE=Ex01/monte_carlo.cpp \
		EXE_FILE=Ex01/monte_carlo \
		Task1_VARIANT=$(val) \
		$(MAKE) -e --no-print-directory run_Task1; \
	)

.PHONY: Task1_OMP_NUM_THREADS
Task1_OMP_NUM_THREADS:
	$(foreach val,$(Task1_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex01/monte_carlo.cpp \
		EXE_FILE=Ex01/monte_carlo \
		Task1_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: compile_Task1
compile_Task1:
	$(Task1_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task1_CFLAGS)  $(Task1_OPTIMIZATION_LEVEL) -DVARIANT=$(Task1_VARIANT)


# ------ Recipes for Task2 (Ex02/modifiedCode.c) ---------------------------------- #

.PHONY: Task2
Task2:
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		$(foreach OMP_NUM_THREADS, $(Task2_OMP_NUM_THREADS_VALUES), \
			$(foreach NUM_SAMPLES, $(Task2_NUM_SAMPLES_VALUES), \
				Task2_OPTIMIZATION_LEVEL=$(opt) \
				COMPILE_FILE=Ex02/modifiedCode.c \
				EXE_FILE=Ex02/modifiedCode \
				Task2_OMP_NUM_THREADS=$(OMP_NUM_THREADS) \
				Task2_NUM_SAMPLES=$(NUM_SAMPLES) \
				$(MAKE) -e --no-print-directory run_Task2; \
			) \
		) \
	)

.PHONY: run_Task2
run_Task2:
	$(MAKE) -e --no-print-directory compile_Task2
ifneq ($(TARGET),LCC)
	OMP_NUM_THREADS=$(Task2_OMP_NUM_THREADS) $(Task2_PROFILER) ./$(EXE_FILE)
else
#	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: Task2_OPT
Task2_OPT: 
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/modifiedCode.c \
		EXE_FILE=Ex02/modifiedCode \
		Task2_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2; \
	)

.PHONY: Task2_NUM_SAMPLES
Task2_NUM_SAMPLES: 
	$(foreach val,$(Task2_NUM_SAMPLES_VALUES), \
		COMPILE_FILE=Ex02/modifiedCode.c \
		EXE_FILE=Ex02/modifiedCode \
		Task2_NUM_SAMPLES=$(val) \
		$(MAKE) -e --no-print-directory run_Task2; \
	)

.PHONY: Task2_OMP_NUM_THREADS
Task2_OMP_NUM_THREADS:
	$(foreach val,$(Task2_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/modifiedCode.c \
		EXE_FILE=Ex02/modifiedCode \
		Task2_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2;\
	)

.PHONY: compile_Task2
compile_Task2:
	$(Task2_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_CFLAGS)  $(Task2_OPTIMIZATION_LEVEL) -DNUM_SAMPLES=$(Task2_NUM_SAMPLES)


# ------ Recipes for Task4 (Ex02/originalCode.c) ---------------------------------- #

.PHONY: Task4
Task4:
	@ $(foreach opt,$(Task4_OPTIMIZATION_LEVELS), \
		$(foreach OMP_NUM_THREADS, $(Task4_OMP_NUM_THREADS_VALUES), \
			$(foreach NUM_SAMPLES, $(Task4_NUM_SAMPLES_VALUES), \
				Task4_OPTIMIZATION_LEVEL=$(opt) \
				COMPILE_FILE=Ex02/originalCode.c \
				EXE_FILE=Ex02/originalCode \
				Task4_OMP_NUM_THREADS=$(OMP_NUM_THREADS) \
				Task4_NUM_SAMPLES=$(NUM_SAMPLES) \
				$(MAKE) -e --no-print-directory run_Task4; \
			) \
		) \
	)

.PHONY: run_Task4
run_Task4:
	$(MAKE) -e --no-print-directory compile_Task4
ifneq ($(TARGET),LCC)
	OMP_NUM_THREADS=$(Task4_OMP_NUM_THREADS) $(Task4_PROFILER) ./$(EXE_FILE)
else
#	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: Task4_OPT
Task4_OPT: 
	@ $(foreach opt,$(Task4_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/originalCode.c \
		EXE_FILE=Ex02/originalCode \
		Task4_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task4; \
	)

.PHONY: Task4_NUM_SAMPLES
Task4_NUM_SAMPLES: 
	$(foreach val,$(Task4_NUM_SAMPLES_VALUES), \
		COMPILE_FILE=Ex02/originalCode.c \
		EXE_FILE=Ex02/originalCode \
		Task4_NUM_SAMPLES=$(val) \
		$(MAKE) -e --no-print-directory run_Task4; \
	)

.PHONY: Task4_OMP_NUM_THREADS
Task4_OMP_NUM_THREADS:
	$(foreach val,$(Task4_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/originalCode.c \
		EXE_FILE=Ex02/originalCode \
		Task4_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task4;\
	)

.PHONY: compile_Task4
compile_Task4:
	$(Task4_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task4_CFLAGS)  $(Task4_OPTIMIZATION_LEVEL) -DNUM_SAMPLES=$(Task4_NUM_SAMPLES)


# ------ Recipes for Task3 (Ex03/monte_carlo.cpp) ---------------------------------- #

.PHONY: Task3
Task3:
	@ $(foreach opt,$(Task3_OPTIMIZATION_LEVELS), \
		$(foreach OMP_NUM_THREADS, $(Task3_OMP_NUM_THREADS_VALUES), \
			$(foreach VARIANT, $(Task3_VARIANT_VALUES), \
				Task3_OPTIMIZATION_LEVEL=$(opt) \
				COMPILE_FILE=Ex03/monte_carlo.cpp \
				EXE_FILE=Ex03/monte_carlo \
				Task3_OMP_NUM_THREADS=$(OMP_NUM_THREADS) \
				Task3_VARIANT=$(VARIANT) \
				$(MAKE) -e --no-print-directory run_Task3; \
			) \
		) \
	)

.PHONY: run_Task3
run_Task3:
	$(MAKE) -e --no-print-directory compile_Task3
ifneq ($(TARGET),LCC)
	OMP_NUM_THREADS=$(Task3_OMP_NUM_THREADS) $(Task3_PROFILER) ./$(EXE_FILE)
else
#	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: Task3_OPT
Task3_OPT: 
	@ $(foreach opt,$(Task3_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex03/monte_carlo.cpp \
		EXE_FILE=Ex03/monte_carlo \
		Task3_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task3; \
	)

.PHONY: Task3_VARIANT
Task3_VARIANT: 
	$(foreach val,$(Task3_VARIANT_VALUES), \
		COMPILE_FILE=Ex03/monte_carlo.cpp \
		EXE_FILE=Ex03/monte_carlo \
		Task3_VARIANT=$(val) \
		$(MAKE) -e --no-print-directory run_Task3; \
	)

.PHONY: Task3_OMP_NUM_THREADS
Task3_OMP_NUM_THREADS:
	$(foreach val,$(Task3_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex03/monte_carlo.cpp \
		EXE_FILE=Ex03/monte_carlo \
		Task3_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task3;\
	)

.PHONY: compile_Task3
compile_Task3:
	$(Task3_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task3_CFLAGS)  $(Task3_OPTIMIZATION_LEVEL) -DVARIANT=$(Task3_VARIANT)

