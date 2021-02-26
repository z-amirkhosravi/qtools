CC = g++

CCFLAGS = -g -Wall -fPIC

SRCDIR = src
BINDIR = bin

INC = `python3-config --includes`
LDFLAGS = `python3-config --ldflags --embed` 
TARGET = tmodule

default: $(TARGET)

$(TARGET):
	$(CC) $(CCFLAGS) $(INC) $(SRCDIR)/$(TARGET).cc -shared -o $(BINDIR)/$(TARGET) $(LDFLAGS) -fno-lto
test:   simple_mc.o pricing.o
	$(CC) $(CCFLAGS) $(INC) $(SRCDIR)/test.cc -o $(BINDIR)/test $(LDFLAGS) -fno-lto
pricing.o: pricing.h simple_mc.h pricing.cc

simple_mc.o: simple_mc.h $(SRCDIR)/simple_mc.cc
clean:
	$(RM)$ $(BINDIR)/$(TARGET)
