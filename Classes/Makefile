CC = g++ 
KDIR := ..
DEPS = ../Classes/

output: CCamera.o CSensor.o main.o CHb100.o CSPOT.o CServer.o sqlite3.o
	$(CC) -I $(KDIR) $(DEPS)CCamera.o $(DEPS)CSensor.o $(DEPS)main.o $(DEPS)CHb100.o $(DEPS)CSPOT.o 
	$(DEPS)CServer.o $(DEPS)sqlite3.o -o $(DEPS)spot.elf

%.o: %.cpp
	$(CC) -c -I $(KDIR) $< -o $(DEPS)$@ 

clean:
	rm $(DEPS)*.o $(DEPS)spot.elf

run:
	./$(DEPS)spot.elf
