CC = g++ 
CFLAGS = -g -l.
DEPS = ..CServer.h sqlite3.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
CServer: main.o CServer.o sqlite3.o
	$(CC) -o CServer main.o CServer.o sqlite3.o
