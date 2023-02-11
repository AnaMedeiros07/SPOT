CXX = aarch64-buildroot-linux-uclibc-g++

LIBS = home/francisco/buildroot/buildroot-2022.08/output/host/aarch64-buildroot-linux-uclibc/sysroot/usr/lib

LDLIBS = -lsqlite3 -lpthread

export PATH = /home/francisco/buildroot/buildroot-2022.08/output/host/bin/

objects = CCamera.o CSensor.o main.o CHb100.o CSPOT.o CDatabase.o

SPOT.elf: $(objects)
	$(CXX) $(objects) -L $(LIBS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@ 

.PHONY : clean
clean:
	$(RM) -f SPOT.elf $(objects)