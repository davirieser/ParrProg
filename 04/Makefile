
.NOTPARALLEL:

STD_CFLAGS= -Wall
COMPILE_FILE?=
EXE_FILE?=
TARGET=LOCAL
# ------ Configuration Values for Task1 (Ex01/threadAffinity.cpp) ---------------------------------- #

Task1_FILE := Ex01/threadAffinity.cpp
Task1_EXE := Ex01/threadAffinity.cpp
Task1_LINKS := -fopenmp
Task1_OPTIMIZATION_LEVEL ?= -O3
Task1_OPTIMIZATION_LEVELS := -O3
Task1_COMPILER ?= g++
	Task1_OMP_NUM_THREADS ?= 4
	Task1_OMP_NUM_THREADS_VALUES = 4
	Task1_OMP_PLACES ?= threads
	Task1_OMP_PLACES_VALUES = threads cores sockets numa
	Task1_OMP_PROC_BIND ?= false
	Task1_OMP_PROC_BIND_VALUES = false true spread close master explicit
Task1_CFLAGS=$(STD_CFLAGS)  $(Task1_LINKS)

# ------ Configuration Values for Task3_serial (Ex03/serial.c) ---------------------------------- #

Task3_serial_FILE := Ex03/serial.c
Task3_serial_EXE := Ex03/serial.c
Task3_serial_LINKS := -fopenmp
Task3_serial_OPTIMIZATION_LEVEL ?= -Ofast
Task3_serial_OPTIMIZATION_LEVELS := -Ofast
Task3_serial_COMPILER ?= gcc
Task3_serial_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task3_serial_LINKS)

# ------ Configuration Values for Task3_parallel (Ex03/parallel.c) ---------------------------------- #

Task3_parallel_FILE := Ex03/parallel.c
Task3_parallel_EXE := Ex03/parallel.c
Task3_parallel_LINKS := -fopenmp
Task3_parallel_OPTIMIZATION_LEVEL ?= -Ofast
Task3_parallel_OPTIMIZATION_LEVELS := -Ofast
Task3_parallel_COMPILER ?= gcc
	Task3_parallel_OMP_NUM_THREADS ?= 1
	Task3_parallel_OMP_NUM_THREADS_VALUES = 1 8
Task3_parallel_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task3_parallel_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1 Task3_serial Task3_parallel

# ------ Recipes for Task1 (Ex01/threadAffinity.cpp) ---------------------------------- #

.PHONY: Task1
Task1:
	@ $(foreach opt,$(Task1_OPTIMIZATION_LEVELS), \
		$(foreach Task1_OMP_NUM_THREADS, $(Task1_OMP_NUM_THREADS_VALUES), \
			$(foreach Task1_OMP_PLACES, $(Task1_OMP_PLACES_VALUES), \
				$(foreach Task1_OMP_PROC_BIND, $(Task1_OMP_PROC_BIND_VALUES), \
					export OMP_NUM_THREADS=$(Task1_OMP_NUM_THREADS) &&  \
					export OMP_PLACES=$(Task1_OMP_PLACES) &&  \
					export OMP_PROC_BIND=$(Task1_OMP_PROC_BIND) &&  \
					COMPILE_FILE=Ex01/threadAffinity.cpp \
					EXE_FILE=Ex01/threadAffinity \
					OPTIMIZATION_LEVEL=$(opt) \
					$(MAKE) -e --no-print-directory run_Task1; \
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

.PHONY: Task1_OMP_PLACES
Task1_OMP_PLACES:
	$(foreach val,$(Task1_OMP_PLACES_VALUES), \
		COMPILE_FILE=Ex01/threadAffinity.cpp \
		EXE_FILE=Ex01/threadAffinity \
		Task1_OMP_PLACES=$(val) \
		$(MAKE) -e --no-print-directory run_Task1;\
	)

.PHONY: Task1_OMP_PROC_BIND
Task1_OMP_PROC_BIND:
	$(foreach val,$(Task1_OMP_PROC_BIND_VALUES), \
		COMPILE_FILE=Ex01/threadAffinity.cpp \
		EXE_FILE=Ex01/threadAffinity \
		Task1_OMP_PROC_BIND=$(val) \
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


# ------ Recipes for Task3_serial (Ex03/serial.c) ---------------------------------- #

.PHONY: Task3_serial
Task3_serial:
	@ $(foreach opt,$(Task3_serial_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex03/serial.c \
		EXE_FILE=Ex03/serial \
		OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task3_serial; \
	)

.PHONY: Task3_serial_OPT
Task3_serial_OPT: 
	@ $(foreach opt,$(Task3_serial_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex03/serial.c \
		EXE_FILE=Ex03/serial \
		Task3_serial_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task3_serial; \
	)

.PHONY: run_Task3_serial
run_Task3_serial:
	$(MAKE) -e --no-print-directory compile_Task3_serial
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task3_serial
compile_Task3_serial:
	$(Task3_serial_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task3_serial_CFLAGS)  $(Task3_serial_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task3_parallel (Ex03/parallel.c) ---------------------------------- #

.PHONY: Task3_parallel
Task3_parallel:
	@ $(foreach opt,$(Task3_parallel_OPTIMIZATION_LEVELS), \
		$(foreach Task3_parallel_OMP_NUM_THREADS, $(Task3_parallel_OMP_NUM_THREADS_VALUES), \
			export OMP_NUM_THREADS=$(Task3_parallel_OMP_NUM_THREADS) &&  \
			COMPILE_FILE=Ex03/parallel.c \
			EXE_FILE=Ex03/parallel \
			OPTIMIZATION_LEVEL=$(opt) \
			$(MAKE) -e --no-print-directory run_Task3_parallel; \
		) \
	)

.PHONY: Task3_parallel_OPT
Task3_parallel_OPT: 
	@ $(foreach opt,$(Task3_parallel_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex03/parallel.c \
		EXE_FILE=Ex03/parallel \
		Task3_parallel_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task3_parallel; \
	)

.PHONY: Task3_parallel_OMP_NUM_THREADS
Task3_parallel_OMP_NUM_THREADS:
	$(foreach val,$(Task3_parallel_OMP_NUM_THREADS_VALUES), \
		COMPILE_FILE=Ex03/parallel.c \
		EXE_FILE=Ex03/parallel \
		Task3_parallel_OMP_NUM_THREADS=$(val) \
		$(MAKE) -e --no-print-directory run_Task3_parallel;\
	)

.PHONY: run_Task3_parallel
run_Task3_parallel:
	$(MAKE) -e --no-print-directory compile_Task3_parallel
ifneq ($(TARGET),LCC)
	./$(EXE_FILE)
else
	sbatch ../job.sh $(EXE_FILE)
endif

.PHONY: compile_Task3_parallel
compile_Task3_parallel:
	$(Task3_parallel_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task3_parallel_CFLAGS)  $(Task3_parallel_OPTIMIZATION_LEVEL) 

