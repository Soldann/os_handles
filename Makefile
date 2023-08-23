CC=gcc
CFLAGS = -g -Wall
BUILDDIR = build

SOURCES = main.c handle.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = main

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o *~ $(TARGET)