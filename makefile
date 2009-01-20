COMPILER=gcc
COM_OPTS=-std=c99 -Wall
LNK_OPTS=-lreadline
MAIN_FILE=main

PRG_NAME=shelldon
MODULES=parser.o cmd.o tools.o

# Rules to always execute.
.PHONY: all clean mrproper

# Default action.
all: $(PRG_NAME)

# Delete the object files.
clean:
	rm -f *.o

# Delete the object files and the compiled program.
mrproper: clean
	rm $(PRG_NAME)

# Linkage.
$(PRG_NAME): $(MAIN_FILE).o $(MODULES)
	$(COMPILER) $(LNK_OPTS) -o $@ $^

main.o: src/main.c src/cmd.h src/parser.h src/tools.h
	$(COMPILER) $(COM_OPTS) -c $<

cmd.o: src/cmd.c src/cmd.h src/parser.h
	$(COMPILER) $(COM_OPTS) -c $<

# Generic rules for simple module compilation.
%.o: src/%.c src/%.h
	$(COMPILER) $(COM_OPTS) -c $<

%.o: src/%.c
	$(COMPILER) $(COM_OPTS) -c $<

