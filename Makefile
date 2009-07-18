COMPILER=gcc
#COM_OPTS=-std=gnu99 -Wall -Winline -Wconversion -pedantic -O3 -D_GNU_SOURCE -DDISABLE_DEBUG -DDISABLE_ASSERTS
COM_OPTS=-std=gnu99 -Wall -Winline -Wconversion -pedantic -ggdb -O -DDISABLE_DEBUG
LNK_OPTS=-lreadline
MAIN_FILE=main

PRG_NAME=shelldon
MODULES=cmd.o shell.o tools.o version.o array.o string.o object.o expression.o expression_list.o

# Rules to always execute.
.PHONY: all clean mrproper

# Default action.
all: $(PRG_NAME)

# Delete the object files.
clean:
	rm -f $(MAIN_FILE).o $(MODULES)

# Delete the object files and the compiled program.
mrproper: clean
	rm $(PRG_NAME)

# Linkage.
$(PRG_NAME): $(MAIN_FILE).o $(MODULES)
	$(COMPILER) $(LNK_OPTS) -o $@ $^

# Compilation.
main.o: src/main.c src/shell.h src/version.h
	$(COMPILER) $(COM_OPTS) -c $<

cmd.o: src/cmd.c src/cmd.h src/shell.h src/tools.h src/version.h
	$(COMPILER) $(COM_OPTS) -c $<

shell.o: src/shell.c src/cmd.h src/shell.h src/tools.h src/version.h
	$(COMPILER) $(COM_OPTS) -c $<

array.o: src/array.c src/array.h src/object.h
	$(COMPILER) $(COM_OPTS) -c $<

string.o: src/string.c src/string.h src/object.h
	$(COMPILER) $(COM_OPTS) -c $<

# Generic rules for simple module compilation.
%.o: src/%.c src/%.h
	$(COMPILER) $(COM_OPTS) -c $<

%.o: src/%.c
	$(COMPILER) $(COM_OPTS) -c $<

