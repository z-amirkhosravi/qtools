CC = g++

CCFLAGS = -g -Wall

SRCDIR = src
BINDIR = bin

TARGET = hello

default: $(TARGET)

$(TARGET):
	$(CC) $(CCFLAGS) $(SRCDIR)/$(TARGET).cpp -o $(BINDIR)/$(TARGET)
clean:
	$(RM)$ $(BINDIR)/$(TARGET)
