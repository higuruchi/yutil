.PHONY: default clean

PRG = yutil
OBJ = main.o err.o

CFLAGS = -g3 -O0
CC = gcc $(CFLAGS)

yutil: main.o err.o
	$(CC) -o $(PRG) $(OBJ)

main.o:
	$(CC) -c main.c 

err.o:
	$(CC) -c err.c 

clean:
	rm -rf $(PRG) $(OBJ)
