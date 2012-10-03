CXX := g++
CXXFLAGS := -I.. -std=c++11 -Wall -ftemplate-depth=11111

all: euler1

clean:
	rm euler1

euler1: Euler1.cpp ../FunTs.h
	$(CXX) $(CXXFLAGS) -o $@ $<
