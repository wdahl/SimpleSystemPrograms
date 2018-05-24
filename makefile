# makefile
all:	filegrep lsal lsal2
filegrep:	filegrep.c
			gcc -o filegrep filegrep.c
lsal:	lsal.c
		gcc -o lsal lsal.c
lsal2:	lsal2.c
		gcc -o lsal2 lsal2.c