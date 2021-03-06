CC=gcc

CFLAGS=-Wall -Wextra -g3
LFLAGS=

OBJS=dijkstra.o mm.o queue.o
DEPS=mm.h queue.h
LIBS=

BIN=dijkstra_test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(BIN)
