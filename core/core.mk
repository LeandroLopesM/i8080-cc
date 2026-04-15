DIR=core
CORE_DEPS = 		\
	$(OBJDIR)/log.o	\

$(OBJDIR)/core.o: $(CORE_DEPS) $(OBJDIR)/obj
	ld --relocatable $(CORE_DEPS) -o $(OBJDIR)/core.o

$(OBJDIR)/log.o: $(DIR)/log.c $(BINDIR)/obj
	$(CC) $(DIR)/log.c -c -o $(OBJDIR)/log.o $(CFLAGS)