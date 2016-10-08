include ../Makefile.rules

OBJS =	edge.o			\
	enum.o			\
	graph.o			\
	schema.o		\
	string_pool.o		\
	tuple.o			\
	tuple_print.o		\
	tuple_set.o		\
	tuple_util.o		\
	vertex.o		\
	vertexid.o

TGT = libdb.a

all: $(OBJS)
	ar -r $(TGT) $(OBJS)

clean:
	rm -f $(TGT) $(OBJS)

wc: clean
	wc -l *.c
