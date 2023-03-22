CFLAGS = -std=gnu11 -Wall -Wextra -lm -pthread

EXE=monte_carlo

.PHONY: all
all: $(EXE)  

.PHONY: clean
clean:
	$(RM) $(EXE)

$(EXE): $(EXE).c
	$(CC) -o $(EXE) $(CFLAGS) $(EXE).c
