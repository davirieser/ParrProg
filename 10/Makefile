
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LCC
# ------ Configuration Values for Task2_1 (Ex02/1-original.c) ---------------------------------- #

Task2_1_FILE := Ex02/1-original.c
Task2_1_EXE := Ex02/1-original.c
Task2_1_LINKS := -fopenmp
Task2_1_OPTIMIZATION_LEVEL ?= -Ofast
Task2_1_OPTIMIZATION_LEVELS := -Ofast
Task2_1_COMPILER ?= gcc
	Task2_1_OMP_NUM_THREADS ?= 1
	Task2_1_OMP_NUM_THREADS_VALUES = 1 2 4 8 16 32
Task2_1_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_1_LINKS)

# ------ Configuration Values for Task2_2 (Ex02/1-modified.c) ---------------------------------- #

Task2_2_FILE := Ex02/1-modified.c
Task2_2_EXE := Ex02/1-modified.c
Task2_2_LINKS := -fopenmp
Task2_2_OPTIMIZATION_LEVEL ?= -Ofast
Task2_2_OPTIMIZATION_LEVELS := -Ofast
Task2_2_COMPILER ?= gcc
	Task2_2_OMP_NUM_THREADS ?= 1
	Task2_2_OMP_NUM_THREADS_VALUES = 1 2 4 8 16 32
Task2_2_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_2_LINKS)

# ------ Configuration Values for Task2_3 (Ex02/2-original.c) ---------------------------------- #

Task2_3_FILE := Ex02/2-original.c
Task2_3_EXE := Ex02/2-original.c
Task2_3_LINKS := -fopenmp
Task2_3_OPTIMIZATION_LEVEL ?= -Ofast
Task2_3_OPTIMIZATION_LEVELS := -Ofast
Task2_3_COMPILER ?= gcc
	Task2_3_OMP_NUM_THREADS ?= 1
	Task2_3_OMP_NUM_THREADS_VALUES = 1 2 4 8 16 32
Task2_3_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_3_LINKS)

# ------ Configuration Values for Task2_4 (Ex02/2-modified.c) ---------------------------------- #

Task2_4_FILE := Ex02/2-modified.c
Task2_4_EXE := Ex02/2-modified.c
Task2_4_LINKS := -fopenmp
Task2_4_OPTIMIZATION_LEVEL ?= -Ofast
Task2_4_OPTIMIZATION_LEVELS := -Ofast
Task2_4_COMPILER ?= gcc
	Task2_4_OMP_NUM_THREADS ?= 1
	Task2_4_OMP_NUM_THREADS_VALUES = 1 2 4 8 16 32
Task2_4_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_4_LINKS)

# ------ Configuration Values for Task2_5 (Ex02/3-original.c) ---------------------------------- #

Task2_5_FILE := Ex02/3-original.c
Task2_5_EXE := Ex02/3-original.c
Task2_5_LINKS := -fopenmp
Task2_5_OPTIMIZATION_LEVEL ?= -Ofast
Task2_5_OPTIMIZATION_LEVELS := -Ofast
Task2_5_COMPILER ?= gcc
	Task2_5_OMP_NUM_THREADS ?= 1
	Task2_5_OMP_NUM_THREADS_VALUES = 1 2 4 8 16 32
Task2_5_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_5_LINKS)

# ------ Configuration Values for Task2_6 (Ex02/3-modified.c) ---------------------------------- #

Task2_6_FILE := Ex02/3-modified.c
Task2_6_EXE := Ex02/3-modified.c
Task2_6_LINKS := -fopenmp
Task2_6_OPTIMIZATION_LEVEL ?= -Ofast
Task2_6_OPTIMIZATION_LEVELS := -Ofast
Task2_6_COMPILER ?= gcc
	Task2_6_OMP_NUM_THREADS ?= 1
	Task2_6_OMP_NUM_THREADS_VALUES = 1 2 4 8 16 32
Task2_6_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_6_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task2_1 Task2_2 Task2_3 Task2_4 Task2_5 Task2_6

# ------ Recipes for Task2_1 (Ex02/1-original.c) ---------------------------------- #

.PHONY: Task2_1
Task2_1:
	@ $(foreach opt,$(Task2_1_OPTIMIZATION_LEVELS), \
		$(foreach Task2_1_OMP_NUM_THREADS, $(Task2_1_OMP_NUM_THREADS_VALUES), \
			export OMP_NUM_THREADS=$(Task2_1_OMP_NUM_THREADS) &&  \
			COMPILE_FILE=Ex02/1-original.c \
			EXE_FILE=Ex02/1-original \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task2_1; \
		) \
	)

.PHONY: Task2_1_OPT
Task2_1_OPT: 
	@ $(foreach opt,$(Task2_1_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/1-original.c \
		EXE_FILE=Ex02/1-original \
		Task2_1_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2_1; \
	)

.PHONY: Task2_1_OMP_NUM_THREADS
Task2_1_OMP_NUM_THREADS:
	$(foreach val,$(Task2_1_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/1-original.c \
		EXE_FILE=Ex02/1-original \
		Task2_1_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2_1;\
	)

.PHONY: run_Task2_1
run_Task2_1:
	$(MAKE) -e --no-print-directory compile_Task2_1
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task2_1
compile_Task2_1:
	$(Task2_1_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_1_CFLAGS)  $(Task2_1_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task2_2 (Ex02/1-modified.c) ---------------------------------- #

.PHONY: Task2_2
Task2_2:
	@ $(foreach opt,$(Task2_2_OPTIMIZATION_LEVELS), \
		$(foreach Task2_2_OMP_NUM_THREADS, $(Task2_2_OMP_NUM_THREADS_VALUES), \
			export OMP_NUM_THREADS=$(Task2_2_OMP_NUM_THREADS) &&  \
			COMPILE_FILE=Ex02/1-modified.c \
			EXE_FILE=Ex02/1-modified \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task2_2; \
		) \
	)

.PHONY: Task2_2_OPT
Task2_2_OPT: 
	@ $(foreach opt,$(Task2_2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/1-modified.c \
		EXE_FILE=Ex02/1-modified \
		Task2_2_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2_2; \
	)

.PHONY: Task2_2_OMP_NUM_THREADS
Task2_2_OMP_NUM_THREADS:
	$(foreach val,$(Task2_2_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/1-modified.c \
		EXE_FILE=Ex02/1-modified \
		Task2_2_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2_2;\
	)

.PHONY: run_Task2_2
run_Task2_2:
	$(MAKE) -e --no-print-directory compile_Task2_2
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task2_2
compile_Task2_2:
	$(Task2_2_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_2_CFLAGS)  $(Task2_2_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task2_3 (Ex02/2-original.c) ---------------------------------- #

.PHONY: Task2_3
Task2_3:
	@ $(foreach opt,$(Task2_3_OPTIMIZATION_LEVELS), \
		$(foreach Task2_3_OMP_NUM_THREADS, $(Task2_3_OMP_NUM_THREADS_VALUES), \
			export OMP_NUM_THREADS=$(Task2_3_OMP_NUM_THREADS) &&  \
			COMPILE_FILE=Ex02/2-original.c \
			EXE_FILE=Ex02/2-original \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task2_3; \
		) \
	)

.PHONY: Task2_3_OPT
Task2_3_OPT: 
	@ $(foreach opt,$(Task2_3_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/2-original.c \
		EXE_FILE=Ex02/2-original \
		Task2_3_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2_3; \
	)

.PHONY: Task2_3_OMP_NUM_THREADS
Task2_3_OMP_NUM_THREADS:
	$(foreach val,$(Task2_3_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/2-original.c \
		EXE_FILE=Ex02/2-original \
		Task2_3_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2_3;\
	)

.PHONY: run_Task2_3
run_Task2_3:
	$(MAKE) -e --no-print-directory compile_Task2_3
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task2_3
compile_Task2_3:
	$(Task2_3_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_3_CFLAGS)  $(Task2_3_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task2_4 (Ex02/2-modified.c) ---------------------------------- #

.PHONY: Task2_4
Task2_4:
	@ $(foreach opt,$(Task2_4_OPTIMIZATION_LEVELS), \
		$(foreach Task2_4_OMP_NUM_THREADS, $(Task2_4_OMP_NUM_THREADS_VALUES), \
			export OMP_NUM_THREADS=$(Task2_4_OMP_NUM_THREADS) &&  \
			COMPILE_FILE=Ex02/2-modified.c \
			EXE_FILE=Ex02/2-modified \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task2_4; \
		) \
	)

.PHONY: Task2_4_OPT
Task2_4_OPT: 
	@ $(foreach opt,$(Task2_4_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/2-modified.c \
		EXE_FILE=Ex02/2-modified \
		Task2_4_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2_4; \
	)

.PHONY: Task2_4_OMP_NUM_THREADS
Task2_4_OMP_NUM_THREADS:
	$(foreach val,$(Task2_4_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/2-modified.c \
		EXE_FILE=Ex02/2-modified \
		Task2_4_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2_4;\
	)

.PHONY: run_Task2_4
run_Task2_4:
	$(MAKE) -e --no-print-directory compile_Task2_4
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task2_4
compile_Task2_4:
	$(Task2_4_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_4_CFLAGS)  $(Task2_4_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task2_5 (Ex02/3-original.c) ---------------------------------- #

.PHONY: Task2_5
Task2_5:
	@ $(foreach opt,$(Task2_5_OPTIMIZATION_LEVELS), \
		$(foreach Task2_5_OMP_NUM_THREADS, $(Task2_5_OMP_NUM_THREADS_VALUES), \
			export OMP_NUM_THREADS=$(Task2_5_OMP_NUM_THREADS) &&  \
			COMPILE_FILE=Ex02/3-original.c \
			EXE_FILE=Ex02/3-original \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task2_5; \
		) \
	)

.PHONY: Task2_5_OPT
Task2_5_OPT: 
	@ $(foreach opt,$(Task2_5_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/3-original.c \
		EXE_FILE=Ex02/3-original \
		Task2_5_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2_5; \
	)

.PHONY: Task2_5_OMP_NUM_THREADS
Task2_5_OMP_NUM_THREADS:
	$(foreach val,$(Task2_5_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/3-original.c \
		EXE_FILE=Ex02/3-original \
		Task2_5_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2_5;\
	)

.PHONY: run_Task2_5
run_Task2_5:
	$(MAKE) -e --no-print-directory compile_Task2_5
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task2_5
compile_Task2_5:
	$(Task2_5_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_5_CFLAGS)  $(Task2_5_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task2_6 (Ex02/3-modified.c) ---------------------------------- #

.PHONY: Task2_6
Task2_6:
	@ $(foreach opt,$(Task2_6_OPTIMIZATION_LEVELS), \
		$(foreach Task2_6_OMP_NUM_THREADS, $(Task2_6_OMP_NUM_THREADS_VALUES), \
			export OMP_NUM_THREADS=$(Task2_6_OMP_NUM_THREADS) &&  \
			COMPILE_FILE=Ex02/3-modified.c \
			EXE_FILE=Ex02/3-modified \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task2_6; \
		) \
	)

.PHONY: Task2_6_OPT
Task2_6_OPT: 
	@ $(foreach opt,$(Task2_6_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/3-modified.c \
		EXE_FILE=Ex02/3-modified \
		Task2_6_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2_6; \
	)

.PHONY: Task2_6_OMP_NUM_THREADS
Task2_6_OMP_NUM_THREADS:
	$(foreach val,$(Task2_6_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/3-modified.c \
		EXE_FILE=Ex02/3-modified \
		Task2_6_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2_6;\
	)

.PHONY: run_Task2_6
run_Task2_6:
	$(MAKE) -e --no-print-directory compile_Task2_6
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task2_6
compile_Task2_6:
	$(Task2_6_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task2_6_CFLAGS)  $(Task2_6_OPTIMIZATION_LEVEL) 

