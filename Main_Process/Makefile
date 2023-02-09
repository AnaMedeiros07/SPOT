CXX = aarch64-buildroot-linux-uclibc-g++

KDIR := home/francisco/buildroot/buildroot-2022.08/output/host/bin/

LIBS = home/francisco/buildroot/buildroot-2022.08/output/host/aarch64-buildroot-linux-uclibc/sysroot/usr/lib

LDLIBS = -lsqlite3 -lpthread

export PATH = /home/francisco/buildroot/buildroot-2022.08/output/host/bin/

objects = CCamera.o CSensor.o main.o CHb100.o CSPOT.o CDatabase.o

#$(objects): %.o: %.cpp

SPOT.elf: $(objects)
	$(CXX) $(objects) -L $(LIBS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) -c $< -o $@ 

.PHONY : clean
clean:
	$(RM) -f SPOT.elf $(objects)