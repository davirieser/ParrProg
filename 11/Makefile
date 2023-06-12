
NUM_BODIES=5000
TIME_STEP=1
MAX_TIME=100
ARGS=$(NUM_BODIES) $(TIME_STEP) $(MAX_TIME)

OUTPUT_PLOT ?= 1

PARALLEL ?= 1

ifeq "$(PARALLEL)" "1"
FLAGS=-lm -Wall -Wpedantic -fopenmp -O3
else
FLAGS=-lm -Wall -Wpedantic -O3
endif

%: %.c
	@ $(CC) $^ -o $@ $(FLAGS) -DOUTPUT_PLOT=$(OUTPUT_PLOT)
	@ echo "Running $@ $(ARGS)"
	@ ./$@ $(ARGS)
ifneq "$(OUTPUT_PLOT)" "0"
	@ $(MAKE) -s create_plot
endif


.PHONY: create_plot
create_plot:
	@ echo "Creating Plot"
	@ gnuplot particle.plt

.PHONY: clean
clean:
	@ echo "Cleaning temporary Files"
	@ rm -f data.dat output.gif serial parallel particle.plt

