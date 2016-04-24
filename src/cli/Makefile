INCDIRS = -I../../include
LIBDIRS = -L../db

OBJS =	cli.o		\
	cli_graph.o	\
	lex.o		\
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
