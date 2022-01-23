CC=cc
CFLAGS=-c -Wall -g -std=c99
LDFLAGS=-lreadline
SOURSES=main.c matrix.c list.c
OBJECTS=$(SOURSES:.c=.o)
EXECUTABLE=console_snake

all: $(SOURSES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
.c.o:
	$(CC) $(CFLAGS) $< -o $@
