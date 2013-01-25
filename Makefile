CC = gcc
LIBS = -lm -llua5.1
CFLAGS = -g -Wall -DENABLE_DEBUG
INCLUDE=-I$(PWD)/lib/Nano-Lua-Dictionary/ -I/usr/include/lua5.1/ -I .

EXAMPLES=$(wildcard ./examples/*.c)
TESTS=$(wildcard ./tests/*.c)

all:	clean prepare library

update:
	git submodule update --init --recursive
prepare:
	mkdir -p ./objs/

tests: all
	$(CC) $(CFLAGS) $(INCLUDE) $(TESTS) $(PWD)/libbacktesting.so -o tests/run_$@ -lcunit; \
	
examples: all 
	@$(foreach example, $(EXAMPLES)), \
	    $(CC) $(CFLAGS) $(INCLUDE) $(example) $(PWD)/libbacktesting.so -o $(basename $(example)); \
	)
	
library: lua.o list.o reader.o
	$(CC) $(CFLAGS) $(INCLUDE) -shared -fPIC ./objs/list.o ./objs/lua.o ./objs/reader.o -o libbacktesting.so $(LIBS)
	
reader.o: reader.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC reader.c -o ./objs/reader.o
	
list.o: list.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC list.c -o ./objs/list.o

lua.o: lua.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC lua.c -o ./objs/lua.o

clean:
	rm -fr ./objs/ libbacktesting.so
	rm -fr ./examples/*.o
	rm -fr ./tests/*.o ./tests/run_tests
