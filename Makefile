todo: borrar compile borrar clean ejecutar

compilar: borrar compile borrar clean

p0.o:
	gcc -c -g p0.c

p1.o:
	gcc -c -g p1.c

p2.o:
	gcc -c -g p2.c

p3.o:
	gcc -c -g p3.c

listaF.o:
	gcc -c shellopenfiles.c

listaH.o:
	gcc -c historic.c

listaM.o:
	gcc -c mem.c

listaP.o:
	gcc -c proc.c

compile: p0.o p1.o p2.o p3.o listaF.o listaH.o listaM.o listaP.o
	gcc -Wall -o p0 p0.o p1.o p2.o p3.o shellopenfiles.o historic.o mem.o proc.o
	rm -f *.o

ejecutar:
	./p0

borrar:
	rm -f *.o *.txt p0

valgrind: borrar compile borrar clean
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p0

clean:
	clear