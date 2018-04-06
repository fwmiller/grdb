include ../Makefile.rules

OBJS =	component.o			\
	component_project.o		\
	component_select.o		\
	component_union.o		\
	component_union_structure.o	\
	component_sssp.o		\
	edge.o				\
	edge_read.o			\
	edge_write.o			\
	enum.o				\
	enum_union.o			\
	enum_list_read.o		\
	enum_list_write.o		\
	graph.o				\
	graph_import.o			\
	schema.o			\
	schema_union.o			\
	schema_read.o			\
	schema_write.o			\
	string_pool.o			\
	tuple.o				\
	tuple_print.o			\
	tuple_set.o			\
	tuple_util.o			\
	vertex.o			\
	vertex_read.o			\
	vertex_write.o			\
	vertex_write_from.o		\
	vertexid.o

TGT = libdb.a

all: $(OBJS)
	ar -r $(TGT) $(OBJS)

clean:
	rm -f $(TGT) $(OBJS)

wc: clean
	wc -l *.c
