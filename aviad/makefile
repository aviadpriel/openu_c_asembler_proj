main.out:main.o isSomsing.o testfunction.o prog_struct.o
	gcc -g -ansi -Wall -pedantic main.o isSomsing.o testfunction.o prog_struct.o -o main.out

isSomsing.o:isSomsing.c
	gcc -g -ansi -Wall -pedantic -c isSomsing.c

testfunction.o:testfunction.c
	gcc -g -ansi -Wall -pedantic -c testfunction.c

prog_struct.o:prog_struct.c
	gcc -g -ansi -Wall -pedantic -c prog_struct.c

	main.o:main.c,main.h
	gcc -g -ansi -Wall -pedantic -c main.h main.c