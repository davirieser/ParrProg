
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LOCAL
# ------ Configuration Values for Task1 (Ex01/threadAffinity.cpp) ---------------------------------- #

Task1_FILE := Ex01/threadAffinity.cpp
Task1_EXE := Ex01/threadAffinity.cpp
Task1_LINKS := -fopenmp
Task1_OPTIMIZATION_LEVEL ?= -Ofast
Task1_OPTIMIZATION_LEVELS := -Ofast
Task1_COMPILER ?= g++
	Task1_OMP_NUM_THREADS ?= 1
	Task1_OMP_NUM_THREADS_VALUES = 1 8
	Task1_ENVVAR_VARIANT ?= 0
	Task1_ENVVAR_VARIANT_VALUES = 0 1 2
	Task1_ENVVAR_SIZE_M ?= 12
	Task1_ENVVAR_SIZE_M_VALUES = 12
	Task1_ENVVAR_SIZE_N ?= 12
	Task1_ENVVAR_SIZE_N_VALUES = 12
Task1_CFLAGS=$(STD_CFLAGS)  $(Task1_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1

# ------ Recipes for Task1 (Ex01/threadAffinity.cpp) ---------------------------------- #

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
						COMPILE_FILE=Ex01/threadAffinity.cpp \
						EXE_FILE=Ex01/threadAffinity \
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
		COMPILE_FILE=Ex01/threadAffinity.cpp \
		EXE_FILE=Ex01/threadAffinity \
		Task1_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task1; \
	)

.PHONY: Task1_OMP_NUM_THREADS
Task1_OMP_NUM_THREADS:
	$(foreach val,$(Task1_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex01/threadAffinity.cpp \
		EXE_FILE=Ex01/threadAffinity \
		Task1_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_ENVVAR_VARIANT
Task1_ENVVAR_VARIANT:
	$(foreach val,$(Task1_ENVVAR_VARIANT_VALUES), \
		COMPILE_FILE=Ex01/threadAffinity.cpp \
		EXE_FILE=Ex01/threadAffinity \
		Task1_ENVVAR_VARIANT=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_ENVVAR_SIZE_M
Task1_ENVVAR_SIZE_M:
	$(foreach val,$(Task1_ENVVAR_SIZE_M_VALUES), \
		COMPILE_FILE=Ex01/threadAffinity.cpp \
		EXE_FILE=Ex01/threadAffinity \
		Task1_ENVVAR_SIZE_M=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_ENVVAR_SIZE_N
Task1_ENVVAR_SIZE_N:
	$(foreach val,$(Task1_ENVVAR_SIZE_N_VALUES), \
		COMPILE_FILE=Ex01/threadAffinity.cpp \
		EXE_FILE=Ex01/threadAffinity \
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

