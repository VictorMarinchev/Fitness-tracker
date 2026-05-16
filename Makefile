CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: fitness_tracker

fitness_tracker: main.cpp $(wildcard *.h)
	$(CXX) $(CXXFLAGS) main.cpp -o fitness_tracker

clean:
	rm -f fitness_tracker

.PHONY: all clean