include ../Makefile.rules

OBJS =	component.o		\
	component_project.o	\
	component_select.o	\
	component_join.o	\
	edge.o			\
	enum.o			\
	enum_join.o		\
	graph.o			\
	schema.o		\
	schema_join.o		\
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
