DIR=core
CORE_SOURCES = 		\
	$(BINDIR)/obj/log.o	\

$(BINDIR)/obj/core.o: $(CORE_SOURCES) $(BINDIR)/obj
	ld --relocatable $(CORE_SOURCES) -o $(BINDIR)/obj/core.o

$(BINDIR)/obj/log.o: $(DIR)/log.c $(BINDIR)/obj
	$(CC) $(DIR)/log.c -c -o $(BINDIR)/obj/log.o $(CFLAGS)