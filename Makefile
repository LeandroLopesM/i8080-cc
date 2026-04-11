CC=gcc
CFLAGS+= -Wall -Wextra

BINDIR=bin

include compiler/compiler.mk
include core/core.mk

$(BINDIR):
	mkdir $(BINDIR)
$(BINDIR)/obj: $(BINDIR)
	mkdir $(BINDIR)/obj