# Because we use  the default project, the default  target is “bin/default”, but
# I prefer this one.
default_TARGET := bin/shelldon

# Shelldon depends on the readline library.
LIBRARIES := readline

CFLAGS := -std=gnu99 -pedantic -Wall

# Because it is  an old program, it  has a lots of problems,  disables the extra
# warnings.
EXTRA_WARNINGS := 0

########################################

# If the MGM submodule has not already been checked out, do it.
tools/mgm/mgm.mk:
	git submodule update --init

# Includes MGM.
include tools/mgm/mgm.mk
