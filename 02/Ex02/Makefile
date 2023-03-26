CFLAGS = -Wall -Wextra -lm
LINK := -fopenmp

File := main
File_ext := cpp

VARIANT_LIST := 0 1
VARIANT=0

.PHONY: all
all: main.cpp

$(File): $(File).$(File_ext)
	g++ $^ -o $@ $(STD) $(LINK) $(COMPILE_STD) $(VARIANT)


runall: $(patsubst %,runall-%,$(VARIANT_LIST))
runall-%:
	touch $(File).$(File_ext) ; \
	$(MAKE) VARIANT=$* all run_lcc2_3 ; \

run_lcc2_%:
	for i in $$(seq 1 $*) ; do \
		$(MAKE) run_lcc2 ; \
		sleep 4 ; \
	done

run_lcc2:
	sbatch job.sh
