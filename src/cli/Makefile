INCDIRS = -I../../include
LIBDIRS = -L../db

OBJS =	cli.o			\
	cli_graph.o		\
	cli_graph_edge.o	\
	cli_graph_schema.o	\
	cli_graph_vertex.o	\
	lex.o			\
	main.o

LIBS = -ldb
TGT = db

all: $(OBJS)
	cd ../db; make;
	gcc $(LIBDIRS) -o $(TGT) $(OBJS) $(LIBS)

clean:
	cd ../db; make clean;
	rm -f $(TGT) $(OBJS)

wc: clean
	cd ../db; make wc;
	wc -l *.c

.c.o: $<
	gcc -c -Wall $(INCDIRS) -o $@ $<
