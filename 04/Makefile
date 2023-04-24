
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
Task3_serial_PROFILER ?= Task3_serial_timer
Task3_serial_COMPILER ?= gcc
Task3_serial_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task3_serial_LINKS)

# ------ Configuration Values for Task3_parrallel (Ex03/parrallel.c) ---------------------------------- #

Task3_parrallel_FILE := Ex03/parrallel.c
Task3_parrallel_EXE := Ex03/parrallel.c
Task3_parrallel_LINKS := -fopenmp
Task3_parrallel_OPTIMIZATION_LEVEL ?= -Ofast
Task3_parrallel_OPTIMIZATION_LEVELS := -Ofast
Task3_parrallel_PROFILER ?= Task3_parrallel_timer
Task3_parrallel_COMPILER ?= gcc
Task3_parrallel_CFLAGS=$(STD_CFLAGS) -std=c11 $(Task3_parrallel_LINKS)

# ------ Run all Tasks Recipe ----- #

.PHONY: all
all: Task1 Task3_serial Task3_parrallel Task3_parralel

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
	./$(Task3_serial_PROFILER) ./$(EXE_FILE)
else
	sbatch ../job.sh "timer $(EXE_FILE)"
endif

.PHONY: compile_Task3_serial
compile_Task3_serial:
	$(Task3_serial_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task3_serial_CFLAGS) --shared -fPIC $(Task3_serial_OPTIMIZATION_LEVEL) 


# ------ Recipes for Task3_parrallel (Ex03/parrallel.c) ---------------------------------- #

.PHONY: Task3_parrallel
Task3_parrallel:
	@ $(foreach opt,$(Task3_parrallel_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex03/parrallel.c \
		EXE_FILE=Ex03/parrallel \
		OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task3_parrallel; \
	)

.PHONY: Task3_parrallel_OPT
Task3_parrallel_OPT: 
	@ $(foreach opt,$(Task3_parrallel_OPTIMIZATION_LEVELS), \
		COMPILE_FILE=Ex03/parrallel.c \
		EXE_FILE=Ex03/parrallel \
		Task3_parrallel_OPTIMIZATION_LEVEL=$(opt) \
		$(MAKE) -e --no-print-directory run_Task3_parrallel; \
	)

.PHONY: run_Task3_parrallel
run_Task3_parrallel:
	$(MAKE) -e --no-print-directory compile_Task3_parrallel
ifneq ($(TARGET),LCC)
	./$(Task3_parrallel_PROFILER) ./$(EXE_FILE)
else
	sbatch ../job.sh "timer $(EXE_FILE)"
endif

.PHONY: compile_Task3_parrallel
compile_Task3_parrallel:
	$(Task3_parrallel_COMPILER) $(COMPILE_FILE) -o $(EXE_FILE) $(Task3_parrallel_CFLAGS) --shared -fPIC $(Task3_parrallel_OPTIMIZATION_LEVEL) 

