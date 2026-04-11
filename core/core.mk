DIR=core
CORE_SOURCES = 		\
	$(DIR)/log.o	\
	$(DIR)/calc.o	\

$(DIR)/core.o: $(CORE_SOURCES)
	ld --relocatable $(CORE_SOURCES) -o $(DIR)/core.o

$(DIR)/log.o: $(DIR)/log.c
	$(CC) $(DIR)/log.c -c -o $(DIR)/log.o $(CFLAGS)
$(DIR)/calc.o: $(DIR)/calc.c
	$(CC) $(DIR)/calc.c -c -o $(DIR)/calc.o $(CFLAGS)