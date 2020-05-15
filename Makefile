CC:=gcc

all:
	$(CC) -Wall -g -o readBMP.out readBMP.c

.PHONY:clean
clean:
	rm readBMP.out
