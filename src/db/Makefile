INCDIRS = -I../../include

OBJS =	edge.o			\
	graph.o			\
	schema.o		\
	vertex.o		\
	vertexid.o

TGT = libdb.a

all: $(OBJS)
	ar -r $(TGT) $(OBJS)

clean:
	rm -f $(TGT) $(OBJS)

wc: clean
	wc -l *.c

.c.o: $<
	gcc -c -Wall $(INCDIRS) -o $@ $<
