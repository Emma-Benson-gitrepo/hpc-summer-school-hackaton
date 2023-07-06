ARGS= -fopenmp
all: occ patt

occ : occ.c
	gcc $(ARGS) -o occ occ.c

occ : patt.c
	gcc $(ARGS) -o patt patt.c
clean :
	rm occ patt
