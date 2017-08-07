main.out:main.o isSomsing.o testfunction.o prog_struct.o first_run.o
	gcc -g -ansi -Wall -pedantic main.o first_run.o isSomsing.o testfunction.o prog_struct.o -o main.out

isSomsing.o:isSomsing.c
	gcc -g -ansi -Wall -pedantic -c isSomsing.c

testfunction.o:testfunction.c
	gcc -g -ansi -Wall -pedantic -c testfunction.c

first_run.o:first_run.c struct.h
	gcc -g -ansi -Wall -pedantic -c first_run.c

prog_struct.o:prog_struct.c
	gcc -g -ansi -Wall -pedantic -c prog_struct.c

main.o:main.c struct.h
	gcc -g -ansi -Wall -pedantic -c  main.c

#cleanUp
clean:
	rm *.o *~