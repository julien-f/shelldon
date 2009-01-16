COMPILER=gcc
COM_OPTS=-std=c99 -Wall
LNK_OPTS=-lreadline
MAIN_FILE=main

PRG_NAME=shelldone
MODULES=parser.o

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

# Generic rules for simple module compilation.
%.o: %.c %.h
	$(COMPILER) $(COM_OPTS) -c $<

%.o: %.c
	$(COMPILER) $(COM_OPTS) -c $<

