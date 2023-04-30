
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LCC
# ------ Configuration Values for Task1 (Ex01/delannoy.c) ---------------------------------- #

Task1_FILE := Ex01/delannoy.c
Task1_EXE := Ex01/delannoy.c
Task1_LINKS := -fopenmp
Task1_OPTIMIZATION_LEVEL ?= -Ofast
Task1_OPTIMIZATION_LEVELS := -Ofast
Task1_COMPILER ?= gcc
	Task1_OMP_NUM_THREADS ?= 1
	Task1_OMP_NUM_THREADS_VALUES = 1 8
	Task1_ENVVAR_VARIANT ?= 0
	Task1_ENVVAR_VARIANT_VALUES = 0 1 2
	Task1_ENVVAR_SIZE_M ?= 12
	Task1_ENVVAR_SIZE_M_VALUES = 12
	Task1_ENVVAR_SIZE_N ?= 12
	Task1_ENVVAR_SIZE_N_VALUES = 12
Task1_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task1_LINKS)

# ------ Configuration Values for Task2 (Ex02/heat_stencil_2D.c) ---------------------------------- #

Task2_FILE := Ex02/heat_stencil_2D.c
Task2_EXE := Ex02/heat_stencil_2D.c
Task2_LINKS := -fopenmp
Task2_OPTIMIZATION_LEVEL ?= -Ofast
Task2_OPTIMIZATION_LEVELS := -Ofast
Task2_COMPILER ?= gcc
	Task2_OMP_NUM_THREADS ?= 1
	Task2_OMP_NUM_THREADS_VALUES = 1 8
	Task2_ENVVAR_VARIANT ?= 0
	Task2_ENVVAR_VARIANT_VALUES = 0 1
	Task2_ENVVAR_N ?= 10
	Task2_ENVVAR_N_VALUES = 10 20 50 100 200 300 400 500
Task2_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task2_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1 Task2

# ------ Recipes for Task1 (Ex01/delannoy.c) ---------------------------------- #

.PHONY: Task1
Task1:
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		$(foreach Task1_OMP_NUM_THREADS, $(Task1_OMP_NUM_THREADS_VALUES), \
			$(foreach Task1_ENVVAR_VARIANT, $(Task1_ENVVAR_VARIANT_VALUES), \
				$(foreach Task1_ENVVAR_SIZE_M, $(Task1_ENVVAR_SIZE_M_VALUES), \
					$(foreach Task1_ENVVAR_SIZE_N, $(Task1_ENVVAR_SIZE_N_VALUES), \
						export OMP_NUM_THREADS=$(Task1_OMP_NUM_THREADS) &&  \
						export ENVVAR_VARIANT=$(Task1_ENVVAR_VARIANT) &&  \
						export ENVVAR_SIZE_M=$(Task1_ENVVAR_SIZE_M) &&  \
						export ENVVAR_SIZE_N=$(Task1_ENVVAR_SIZE_N) &&  \
						COMPILE_FILE=Ex01/delannoy.c \
						EXE_FILE=Ex01/delannoy \
						OPTIMIZATION_LEVEL=$(opt) \
						$(MAKE) -e --no-print-directory run_Task1; \
					) \
				) \
			) \
		) \
	)

.PHONY: Task1_OPT
Task1_OPT: 
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex01/delannoy.c \
		EXE_FILE=Ex01/delannoy \
		Task1_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task1; \
	)

.PHONY: Task1_OMP_NUM_THREADS
Task1_OMP_NUM_THREADS:
	$(foreach val,$(Task1_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex01/delannoy.c \
		EXE_FILE=Ex01/delannoy \
		Task1_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_ENVVAR_VARIANT
Task1_ENVVAR_VARIANT:
	$(foreach val,$(Task1_ENVVAR_VARIANT_VALUES), \
		COMPILE_FILE=Ex01/delannoy.c \
		EXE_FILE=Ex01/delannoy \
		Task1_ENVVAR_VARIANT=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_ENVVAR_SIZE_M
Task1_ENVVAR_SIZE_M:
	$(foreach val,$(Task1_ENVVAR_SIZE_M_VALUES), \
		COMPILE_FILE=Ex01/delannoy.c \
		EXE_FILE=Ex01/delannoy \
		Task1_ENVVAR_SIZE_M=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_ENVVAR_SIZE_N
Task1_ENVVAR_SIZE_N:
	$(foreach val,$(Task1_ENVVAR_SIZE_N_VALUES), \
		COMPILE_FILE=Ex01/delannoy.c \
		EXE_FILE=Ex01/delannoy \
		Task1_ENVVAR_SIZE_N=$(val) \
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


# ------ Recipes for Task2 (Ex02/heat_stencil_2D.c) ---------------------------------- #

.PHONY: Task2
Task2:
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		$(foreach Task2_OMP_NUM_THREADS, $(Task2_OMP_NUM_THREADS_VALUES), \
			$(foreach Task2_ENVVAR_VARIANT, $(Task2_ENVVAR_VARIANT_VALUES), \
				$(foreach Task2_ENVVAR_N, $(Task2_ENVVAR_N_VALUES), \
					export OMP_NUM_THREADS=$(Task2_OMP_NUM_THREADS) &&  \
					export ENVVAR_VARIANT=$(Task2_ENVVAR_VARIANT) &&  \
					export ENVVAR_N=$(Task2_ENVVAR_N) &&  \
					COMPILE_FILE=Ex02/heat_stencil_2D.c \
					EXE_FILE=Ex02/heat_stencil_2D \
					OPTIMIZATION_LEVEL=$(opt) \
					$(MAKE) -e --no-print-directory run_Task2; \
				) \
			) \
		) \
	)

.PHONY: Task2_OPT
Task2_OPT: 
	@ $(foreach opt,$(Task2_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex02/heat_stencil_2D.c \
		EXE_FILE=Ex02/heat_stencil_2D \
		Task2_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task2; \
	)

.PHONY: Task2_OMP_NUM_THREADS
Task2_OMP_NUM_THREADS:
	$(foreach val,$(Task2_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex02/heat_stencil_2D.c \
		EXE_FILE=Ex02/heat_stencil_2D \
		Task2_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task2;\
	)

.PHONY: Task2_ENVVAR_VARIANT
Task2_ENVVAR_VARIANT:
	$(foreach val,$(Task2_ENVVAR_VARIANT_VALUES), \
		COMPILE_FILE=Ex02/heat_stencil_2D.c \
		EXE_FILE=Ex02/heat_stencil_2D \
		Task2_ENVVAR_VARIANT=$(val) \
		$(MAKE) -e --no-print-directory run_Task2;\
	)

.PHONY: Task2_ENVVAR_N
Task2_ENVVAR_N:
	$(foreach val,$(Task2_ENVVAR_N_VALUES), \
		COMPILE_FILE=Ex02/heat_stencil_2D.c \
		EXE_FILE=Ex02/heat_stencil_2D \
		Task2_ENVVAR_N=$(val) \
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

