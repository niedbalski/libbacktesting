CC = gcc
LIBS = -lm -llua5.1
CFLAGS = -g -Wall -DENABLE_DEBUG
INCLUDE=-I./lib/Nano-Lua-Dictionary/ -I/usr/include/lua5.1/ -I.

all:	clean prepare library

prepare:
	mkdir -p ./objs/

library: lua.o
	$(CC) $(CFLAGS) $(INCLUDE) -shared -fPIC ./objs/lua.o -o libbacktesting.so $(LIBS)

lua.o: lua.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -fPIC lua.c -o ./objs/lua.o

clean:
	rm -fr ./objs/ libbacktesting.so
