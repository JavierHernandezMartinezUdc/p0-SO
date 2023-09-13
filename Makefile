todo: borrar compilar ejecutar

p0.o:
	gcc -c p0.c
# Preguntar -g

compilar: p0.o
	gcc -Wall -o p0 p0.o
	rm -f *.o

ejecutar:
	./p0

borrar:
	rm -f *.o *.txt p0