CC=gcc
CFLAGS=-c -Wall -I. -I "mpack/" -I "src/" -D MPACK_DEBUG=1
#CFLAGS=-c -I. -I "mpack/" -I "src/" -D MPACK_DEBUG=0
LDFLAGS=
SOURCES=src/main.c src/hexDump.c mpack/mpack-common.c mpack/mpack-expect.c mpack/mpack-node.c mpack/mpack-platform.c mpack/mpack-reader.c mpack/mpack-writer.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mpack-run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm mpack-run ./mpack/*.o 2>/dev/null
