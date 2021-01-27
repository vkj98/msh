msh: main.o command_helper.o
	gcc main.o -o msh

main.o: main.c
	gcc -c main.c
	
command_helper.o: command_helper.c command_helper.h 
	gcc -c command_helper.c

clean:
	rm *.o