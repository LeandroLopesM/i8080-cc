BINDIR?=bin
CC_DIR?=compiler

SOURCES = 								\
	$(BINDIR)/obj/encoder.o				\
	$(BINDIR)/obj/argument.o			\
	$(BINDIR)/obj/arrays.o				\
	$(BINDIR)/obj/mappings.o			\
	$(BINDIR)/obj/parser.o				\
	$(BINDIR)/obj/core.o

ifdef BUILD_EXE
$(BINDIR)/compiler.exe: $(SOURCES) $(BINDIR)/obj/compiler.o $(BINDIR)
	$(CC) $(CFLAGS) $(CC_DIR)/8080cc.c -o $(BINDIR)/8080cc $(SOURCES)
endif

$(BINDIR)/obj/compiler.o: $(SOURCES) $(BINDIR)/obj
	ld --relocatable $(SOURCES) -o $(BINDIR)/obj/compiler.o

$(BINDIR)/obj/encoder.o: $(CC_DIR)/encoder/encoder.c $(BINDIR)/obj
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/encoder.c -c -o $(BINDIR)/obj/encoder.o

$(BINDIR)/obj/argument.o: $(CC_DIR)/encoder/argument/argument.c $(BINDIR)/obj
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/argument/argument.c -c -o $(BINDIR)/obj/argument.o

$(BINDIR)/obj/arrays.o: $(CC_DIR)/parser/util/arrays.c $(BINDIR)/obj
	$(CC) $(CFLAGS) $(CC_DIR)/parser/util/arrays.c -c -o $(BINDIR)/obj/arrays.o

$(BINDIR)/obj/parser.o: $(CC_DIR)/parser/parser.c $(BINDIR)/obj
	$(CC) $(CFLAGS) $(CC_DIR)/parser/parser.c -c -o $(BINDIR)/obj/parser.o -Wno-maybe-uninitialized

$(BINDIR)/obj/mappings.o: $(CC_DIR)/parser/util/mappings.c $(BINDIR)/obj
	$(CC) $(CFLAGS) $(CC_DIR)/parser/util/mappings.c -c -o $(BINDIR)/obj/mappings.o
