#
# Daniel A. Silva 820567716

CXX=g++
CXXFLAGS=-std=c++11 -g


pagetable : pagetable.o Source.o byu_tracereader.o
	$(CXX) $(CXXFLAGS) -o pagetable $^

pagetable.o : pagetable.cpp pagetable.h

Source.o : byutr.h output_mode_helpers.h pagetable.h Source.cpp

byu_tracereader.o : byutr.h byu_tracereader.c

output_mode_helpers.o : output_mode_helpers.h output_mode_helpers.c

clean :
	rm *.o pagetable