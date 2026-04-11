CC_DIR=compiler

SOURCES = 				\
	$(CC_DIR)/encoder/encoder.o	\
	$(CC_DIR)/encoder/argument.o	\
	core/core.o

$(CC_DIR)/compiler.exe: $(SOURCES) $(CC_DIR)/compiler.o
	$(CC) $(CFLAGS) $(CC_DIR)/8080cc.c -o 8080cc $(SOURCES)

$(CC_DIR)/compiler.o: $(SOURCES)
	ld --relocatable $(SOURCES) -o $(CC_DIR)/compiler.o

$(CC_DIR)/encoder/encoder.o: $(CC_DIR)/encoder/encoder.c
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/encoder.c -c -o $(CC_DIR)/encoder/encoder.o
$(CC_DIR)/encoder/argument.o: $(CC_DIR)/encoder/argument.c
	$(CC) $(CFLAGS) $(CC_DIR)/encoder/argument.c -c -o $(CC_DIR)/encoder/argument.o
