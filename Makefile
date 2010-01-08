##
# My Great Makefile
#
# Julien Fontanet <julien.fontanet@isonoe.net>
#
# Copyleft 2010
##

##
# This Makefile works only with gcc/g++ (g++ is used for linking).
# The C prefix is ".c".
# The C++ prefix is ".cpp".
##

# Projects list.
PROJECTS := shelldon

# Global configuration.
CFLAGS  += -std=gnu99
CFLAGS  += -pedantic -Wall -Winline -Wconversion -ggdb3
CFLAGS  += -DNDEBUG -fno-strict-aliasing -funroll-loops -O3 -g0
LDFLAGS += -lreadline

# Per project configuration.
shelldon_TARGET  := bin/shelldon
shelldon_SRCS    := $(wildcard src/*.c)



# DO NOT MODIFY ANYTHING AFTER THIS LINE.



.DEFAULT_GOAL  := all
CFLAGS         += -MMD
CXXFLAGS       += -MMD
COMMON_OBJECTS := $(patsubst %.cpp,%.o,$(COMMON_SRCS:.c=.o))
DEPS           := $(COMMON_OBJECTS:.o=.d)

# Rules to always execute.
.PHONY: all clean mrproper

# Generic rule definition.
define PROJECT_TPL
$(1)_OBJECTS := $$(patsubst %.cpp,%.o,$$($(1)_SRCS:.c=.o))
$(1)_DEPS    := $$($(1)_OBJECTS:.o=.d)

DEPS         += $$($(1)_DEPS)
OBJECTS      += $$($(1)_OBJECTS)
TARGETS      += $$($(1)_TARGET)

$(1): $$($(1)_TARGET)

$$($(1)_TARGET): $$($(1)_OBJECTS) $(COMMON_OBJECTS)
	@test -d $$(@D) || mkdir -p $$(@D)
	$(CXX) $(LDFLAGS) $$($(1)_LDFLAGS) -o $$@ $$^

clean-$(1):
	$(RM) $$($(1)_DEPS) $$($(1)_OBJECTS)

mrproper-$(1): clean-$(1)
	$(RM) $$($(1)_TARGET)
endef

# Creates rules for each project.
$(foreach project,$(PROJECTS),$(eval $(call PROJECT_TPL,$(project))))

# Global rules.
all: $(TARGETS)

clean:
	$(RM) $(DEPS) $(COMMON_OBJECTS) $(OBJECTS)

mrproper: clean
	$(RM) $(TARGETS)

# The compilation depends on this Makefile.
$(OBJECTS): Makefile

# Dependencies.
-include $(DEPS)
