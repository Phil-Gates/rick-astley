COMPILER = gcc
MAIN = rick

all: target

target: $(MAIN).c
	$(COMPILER) $(MAIN).c -o $(MAIN)
