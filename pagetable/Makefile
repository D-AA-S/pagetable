#
# Daniel A. Silva 820567716

CXX=g++
CXXFLAGS=-std=c++11 -g


pagetable : pagetable.o Source.o
	$(CXX) $(CXXFLAGS) -o pagetable $^

pagetable.o : pagetable.cpp pagetable.h

Source.o : pagetable.h byutr.h Source.cpp 

clean :
	rm *.o myls

