todo: borrar compilar ejecutar

p0.o:
	gcc -c -g p0.c

p1.o:
	gcc -c -g p1.c

listaF.o:
	gcc -c shellopenfiles.c

listaH.o:
	gcc -c historic.c

listaM.o:
	gcc -c mem.c

compilar: p0.o p1.o listaF.o listaH.o listaM.o
	gcc -Wall -o p0 p0.o p1.o shellopenfiles.o historic.o mem.o
	rm -f *.o

ejecutar:
	./p0

borrar:
	rm -f *.o *.txt p0

valgrind: borrar compilar
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p0