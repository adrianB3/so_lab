copy: copyfile.o
	gcc -o copy copyfile.o

copyfile.o: copyfile.c
	gcc -c copyfile.c

clean:
	rm -f *.o
	rm copy