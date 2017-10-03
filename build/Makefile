CC = gcc
FLAGS = -std=c11 -lm

.PHONY: build clean msdosdir msdosextr

build: msdosdir msdosextr

msdosdir: ByteType.h BootStrap.h DirEntry.h
	@$(CC) $(FLAGS) -o msdosdir msdosdir.c BootStrap.c DirEntry.c

msdosextr: ByteType.h BootStrap.h DirEntry.h FAT.h
	@$(CC) $(FLAGS) -o msdosextr msdosextr.c BootStrap.c DirEntry.c FAT.c

clean:
	@rm -rf msdosdir
	@rm -rf msdosextr
