default: copy

copy.o: copyfile.c
	gcc -c copyfile.c -o copy.o

copy: copy.o
	gcc copy.o -o copy