CC = g++

CCFLAGS = -g -Wall -fPIC

SRCDIR = src
BINDIR = bin

INC = `python3-config --includes`
LDFLAGS = `python3-config --ldflags --embed` -L./
TARGET = qtools_module

default: $(TARGET)

$(TARGET):
	$(CC) $(CCFLAGS) $(INC) $(TARGET).cc -shared -o $(TARGET) $(LDFLAGS) -fno-lto
test:   base.o pricing.o
	$(CC) $(CCFLAGS) $(INC) -I./ test.cc -o test pricing.o base.o $(LDFLAGS) -fno-lto
trunclat: pricing.o base.o pricing.h base.h
	$(CC) $(CCFLAGS) $(INC) -I./ trunclat.cc -o trunclat pricing.o base.o $(LDFLAGS) -fno-lto

pricing.o: pricing.h base.h pricing.cc base.o
	$(CC) $(CCFLAGS) $(INC) -I./ pricing.cc -shared -o pricing.o base.o $(LDFLAGS) -fno-lto
base.o: base.h base.cc
	$(CC) $(CCFLAGS) $(INC) -I./ base.cc -shared -o base.o $(LDFLAGS) -fno-lto
base.o: base.h base.cc
clean:
	$(RM)$ $(BINDIR)/$(TARGET)
