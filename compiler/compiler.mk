BINDIR?=bin
CC_DIR?=compiler

CC_DEPS = 							\
	$(OBJDIR)/encoder.o				\
	$(OBJDIR)/argument.o			\
	$(OBJDIR)/arrays.o				\
	$(OBJDIR)/mappings.o			\
	$(OBJDIR)/parser.o

ifdef BUILD_EXE
$(BINDIR)/compiler.exe: $(CC_DEPS) $(OBJDIR)/compiler.o $(BINDIR) $(OBJDIR)/core.o
	$(CC) $(CFLAGS) $(CC_DIR)/8080cc.c -o $(BINDIR)/8080cc $(CC_DEPS) $(OBJDIR)/core.o
endif

$(OBJDIR)/compiler.o: $(CC_DEPS)
	ld --relocatable $(CC_DEPS) -o $(OBJDIR)/compiler.o

$(OBJDIR)/encoder.o: $(CC_DIR)/encoder/encoder.c
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/encoder.c -c -o $(OBJDIR)/encoder.o

$(OBJDIR)/argument.o: $(CC_DIR)/encoder/argument/argument.c
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/argument/argument.c -c -o $(OBJDIR)/argument.o

$(OBJDIR)/arrays.o: $(CC_DIR)/parser/util/arrays.c
	$(CC) $(CFLAGS) $(CC_DIR)/parser/util/arrays.c -c -o $(OBJDIR)/arrays.o

$(OBJDIR)/parser.o: $(CC_DIR)/parser/parser.c
	$(CC) $(CFLAGS) $(CC_DIR)/parser/parser.c -c -o $(OBJDIR)/parser.o -Wno-maybe-uninitialized

$(OBJDIR)/mappings.o: $(CC_DIR)/parser/util/mappings.c
	$(CC) $(CFLAGS) $(CC_DIR)/parser/util/mappings.c -c -o $(OBJDIR)/mappings.o
