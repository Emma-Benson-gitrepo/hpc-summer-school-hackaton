ARGS= -O2 -fopenmp

default: all
all: occ patt pagerank 

occ : occ.c
	gcc $(ARGS) -o occ occ.c

patt : patt.c
	gcc $(ARGS) -o patt patt.c

uniq : uniq.c
	gcc $(ARGS) -o uniq uniq.c


pagerank : pagerank.c
	gcc $(ARGS) -o pagerank pagerank.c

clean :
	rm occ patt uniq pagerank
