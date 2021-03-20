#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "pagetable.h"

int main(int argc, char** argv)
{
    int arguments = 0;
    if (argc < 2) 
    {
        std::cout << "Please enter a file" << std::endl;
        return -1;
    }

    while (()) 
    {

    }
    //some big logical function that uses the getopt function

    if (arguments % 2) 
    {
        //-n is active, capture arugment after -n
        //subtract 1 from arguments to parse for -o argument, if its greater than 1 then we have -o
    }
    else if(!(arguments%2))
    {
        //execute whatever function argument int represents
    }

    std::cout << "Hello World!\n";
}