CFLAGS = -Wall -Wextra -lm
LINK :=

File := main
File_ext := cpp

VARIANT_LIST := 0 1
VARIANT=0

.PHONY: all
all: main.cpp

$(File): $(File).$(File_ext)
	g++ $^ -o $@ $(STD) $(LINK) $(COMPILE_STD) -DVARIANT=$(VARIANT)


runVar-%:
	touch $(File).$(File_ext) ; \
	$(MAKE) VARIANT=$* $(File) run_lcc2 ; \

run_lcc2:
	sbatch job.sh
