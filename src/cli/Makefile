include ../Makefile.rules

LIBDIRS = -L../db -L../import

OBJS =	bufdump.o 			\
	cli.o				\
	cli_component_print.o		\
	cli_graph.o			\
	cli_graph_component.o		\
	cli_graph_edge.o		\
	cli_graph_enum.o		\
	cli_graph_import.o		\
	cli_graph_new.o			\
	cli_graph_print.o		\
	cli_graph_schema.o		\
	cli_graph_schema_add.o		\
	cli_graph_tuple.o		\
	cli_graph_update_tuples.o	\
	lex.o				\
	main.o

LIBS = -ldb -limport
TGT = db

all: $(OBJS)
	cd ../db; make;
	cd ../import; make;
	$(CC) $(LIBDIRS) $(GDB) -o $(TGT) $(OBJS) $(LIBS)

clean:
	cd ../db; make clean;
	cd ../import; make clean;
	rm -f $(TGT) $(OBJS)

wc: clean
	wc -l ../../include/*.h ../db/*.c ../import/*.c *.c
