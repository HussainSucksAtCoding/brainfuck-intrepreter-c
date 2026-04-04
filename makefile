objects=main.o

interpreter: $(objects)
	gcc $(objects) -o interpreter

main.o: main.c
	gcc -c main.c

clean:
	rm $(objects) 
