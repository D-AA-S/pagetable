#include <iostream>
#include "pagetable.h"

int main(int argc, char** argv)
{
    if (argc < 2) 
    {
        std::cout << "please enter a file" << std::endl;
        return -1;
    }

    std::cout << "Hello World!\n";
}