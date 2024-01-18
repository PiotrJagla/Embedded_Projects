CC=gcc
CFLAGS=-Wall -Wextra -g

CFILES= main.c
OBJECTS = main.o

BINARY=bin

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ -lm -g -lraylib $^

%.o:%.c
	$(CC) $(CFLAGS)  -c -o $@ $^

clean:
	rm -rf $(BINARY) *.o	

	
