BINDIR?=bin
CC_DIR=compiler

SOURCES = 							\
	$(BINDIR)/obj/encoder.o		\
	$(BINDIR)/obj/argument.o	\
	$(BINDIR)/obj/core.o

$(BINDIR)/compiler.exe: $(SOURCES) $(BINDIR)/obj/compiler.o $(BINDIR)
	$(CC) $(CFLAGS) $(CC_DIR)/8080cc.c -o $(BINDIR)/8080cc $(SOURCES)

$(BINDIR)/obj/compiler.o: $(SOURCES) $(BINDIR)/obj
	ld --relocatable $(SOURCES) -o $(BINDIR)/obj/compiler.o

$(BINDIR)/obj/encoder.o: $(CC_DIR)/encoder/encoder.c $(BINDIR)/obj
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/encoder.c -c -o $(BINDIR)/obj/encoder.o

$(BINDIR)/obj/argument.o: $(CC_DIR)/encoder/argument/argument.c $(BINDIR)/obj
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/argument/argument.c -c -o $(BINDIR)/obj/argument.o
