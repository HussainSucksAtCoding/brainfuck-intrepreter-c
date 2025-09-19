objects=main.o

intrepreter: $(objects)
	gcc $(objects) -o intrepreter

main.o: main.c
	gcc -c main.c

clean:
	rm $(objects)
