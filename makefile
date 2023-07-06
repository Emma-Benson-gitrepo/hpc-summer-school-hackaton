ARGS= -fopenmp

default: all
all: occ patt

occ : occ.c
	gcc $(ARGS) -o occ occ.c

patt : patt.c
	gcc $(ARGS) -o patt patt.c
clean :
	rm occ patt
