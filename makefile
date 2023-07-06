all: occ

occ : occ.c
	gcc -o occ occ.c
clean :
	rm occ
