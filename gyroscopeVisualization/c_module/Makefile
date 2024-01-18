BINARY=bin
INCDIRS=. ./src/
CODEDIRS=. ./src

CC=gcc

DEPFLAGS=-MP -MD
CFLAGS=-Wall -Wextra -g $(foreach D, $(INCDIRS), -I$(D)) $(DEPFLAGS)
LIBS=-lm -lraylib -lpthread

CFILES= $(foreach D,$(CODEDIRS), $(wildcard $(D)/*.c))
OBJECTS= $(patsubst %.c, %.o, $(CFILES))
DEPFILES= $(patsubst %.c, %.d, $(CFILES))


all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

%.o:%.c
	$(CC) $(CFLAGS)  -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

	
