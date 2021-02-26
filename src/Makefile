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
test:   simple_mc.o pricing.o
	$(CC) $(CCFLAGS) $(INC) -I./ test.cc -o test pricing.o simple_mc.o $(LDFLAGS) -fno-lto
pricing.o: pricing.h simple_mc.h pricing.cc

simple_mc.o: simple_mc.h simple_mc.cc
clean:
	$(RM)$ $(BINDIR)/$(TARGET)
