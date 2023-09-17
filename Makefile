todo: borrar compilar ejecutar

p0.o:
	gcc -c p0.c
# Preguntar -g

listaF.o:
	gcc -c shellopenfiles.c

listaH.o:
	gcc -c historic.c

compilar: p0.o listaF.o listaH.o
	gcc -Wall -o p0 p0.o shellopenfiles.o historic.o
	rm -f *.o

ejecutar:
	./p0

borrar:
	rm -f *.o *.txt p0

valgrind: borrar compilar
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p0