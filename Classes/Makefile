CC = g++ 
KDIR := ..
DEPS = ../Classes/


output: CCamera.o CSensor.o CHouse.o  main.o
	$(CC) -I $(KDIR) $(DEPS)CCamera.o $(DEPS)CHouse.o $(DEPS)CSensor.o $(DEPS)main.o -o $(DEPS)spot.elf

%.o: %.cpp
	$(CC) -c -I $(KDIR) $< -o $(DEPS)$@ 

clean:
	rm $(DEPS)*.o $(DEPS)spot.elf

run:
	./$(DEPS)spot.elf
