#
# Daniel A. Silva 820567716

CXX=g++
CXXFLAGS=-std=c++11 -g


pagetable : pagetable.o source.o
	$(CXX) $(CXXFLAGS) -o pagetable $^

pagetable.o : pagetable.cpp pagetable.h

source.o : pagetable.h Source.cpp 

clean :
	rm *.o myls

