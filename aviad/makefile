main.out:main.o isSomsing.o direrctiveTestfunction.o prog_struct.o first_run.o second_run.o general.o
	gcc -g -ansi -Wall -pedantic main.o general.o first_run.o second_run.o isSomsing.o direrctiveTestfunction.o prog_struct.o -o main.out

isSomsing.o:isSomsing.c
	gcc -g -ansi -Wall -pedantic -c isSomsing.c

direrctiveTestfunction.o:direrctiveTestfunction.c
	gcc -g -ansi -Wall -pedantic -c direrctiveTestfunction.c

first_run.o:first_run.c struct.h
	gcc -g -ansi -Wall -pedantic -c first_run.c

prog_struct.o:prog_struct.c
	gcc -g -ansi -Wall -pedantic -c prog_struct.c

second_run.o:second_run.c
	gcc -g -ansi -Wall -pedantic -c second_run.c

general.o:general.c identification.h
	gcc -g -ansi -Wall -pedantic -c  general.c

main.o:main.c struct.h
	gcc -g -ansi -Wall -pedantic -c  main.c

#cleanUp
clean:
	rm *.o *~