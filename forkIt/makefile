all: forkIt.exe
	
hello.exe: forkIt.o
	gcc -o forkIt.exe forkIt.o

hello.o: forkIt.c
	gcc -c forkIt.c

clean:
	rm forkIt.o forkIt.exe
