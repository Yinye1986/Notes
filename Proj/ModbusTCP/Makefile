CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
INCDIR = ./inc
SRCDIR = ./src
SRC = $(SRCDIR)/example.cpp $(SRCDIR)/modbusTCP.cpp
TARGET = main

INCLUDES = -I$(INCDIR)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
