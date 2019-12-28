objects = ergasia1.o

all: $(objects)
	gcc -g $(objects) -o ergasia1

ergasia1.o: ergasia1.c
	gcc -g -c -Wall ergasia1.c

clean:
	rm -f $(objects) ergasia1
