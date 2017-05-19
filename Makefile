CXXFLAGS=-I/usr/local/systemc230/include #-O2
LDFLAGS=-L/usr/local/systemc230/lib-linux -Wl,-rpath=/usr/local/systemc230/lib-linux -lsystemc -lm

.PHONY: all clean

all: prodcons

clean:
	$(RM) prodcons
	$(RM) Simulation_Results/*

prodcons: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(LDFLAGS)
