PRG_NAME=shelldon
SOURCES=\
	src/main.c \
	src/cmd.c \
	src/shell.c \
	src/tools.c \
	src/version.c \
	src/array.c \
	src/string.c \
	src/object.c

CFLAGS += -W -Wall -Winline -Wconversion
CFLAGS += -pedantic -std=gnu99 -ggdb
CFLAGS += -fno-strict-aliasing -funroll-loops -O3
#CFLAGS += -DDISABLE_DEBUG -DDISABLE_ASSERTS
LDFLAGS=-lreadline

DEPS=$(patsubst %.c,%.d,$(SOURCES))
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

# Rules to always execute.
.PHONY: all clean mrproper

# Default action.
all: $(PRG_NAME)

# Delete the object files.
clean:
	rm -f $(OBJECTS)

# Delete the object files and the compiled program.
mrproper: clean
	rm $(PRG_NAME)

# Linkage.
$(PRG_NAME): $(OBJECTS)
	@$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -DNDEBUG -MMD -MP -c $< -o $@

-include $(DEPS)
