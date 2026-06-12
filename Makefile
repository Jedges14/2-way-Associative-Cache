CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: cacheTest

cacheTest: cachemain.cpp cachetest.cpp
	$(CXX) $(CXXFLAGS) -o cachetest cachemain.cpp cachetest.cpp

clean:
	rm -f cachetest