CC=gcc
CFLAGS+= -Wall -Wextra

BINDIR=bin
BUILD_EXE=1

include compiler/compiler.mk
include core/core.mk

$(BINDIR):
	mkdir $(BINDIR)
$(BINDIR)/obj: $(BINDIR)
	mkdir -p $(BINDIR)/obj