all: hello_os.c
	gcc -c hello_os.c -o hello_os.o

clean:
	rm hello_os.o

