# Kathryn Shafer 821586552
# Daniel A. Silva 820567716

CXX=g++
CXXFLAGS=-std=c++11 -g
CXXXFLAGS=-std=c99

pagetable : pagetable.o Source.o byu_tracereader.o output_mode_helpers.o 
	$(CXX) $(CXXFLAGS) -o pagetable $^

pagetable.o : pagetable.h pagetable.cpp

output_mode_helpers.o : output_mode_helpers.h output_mode_helpers.c
	$(CXX) $(CXXXFLAGS) -c output_mode_helpers.c $^
byu_tracereader.o : byutr.h byu_tracereader.c

Source.o : byutr.h output_mode_helpers.h pagetable.h Source.cpp

clean :
	rm *.o pagetable