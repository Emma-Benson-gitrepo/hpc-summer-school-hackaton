all: occ patt

occ : occ.c
	gcc -o occ occ.c

occ : patt.c
	gcc -o patt patt.c
clean :
	rm occ patt
