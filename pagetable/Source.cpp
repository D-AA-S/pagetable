#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "pagetable.h"

int main(int argc, char** argv)
{
    int arguments = 0;
    int weOutHere = 0;
    if (argc < 2) 
    {
        std::cout << "Please enter a file" << std::endl;
        return -1;
    }

    while ((weOutHere = getopt(argc, argv, ":n:o:")) != -1) 
    {
        switch (weOutHere)
        {
        case 'n':
            std::cout << "bah" << std::endl;
            break;
        case 'o':
            std::cout << "FAH" << std::endl;
            break;
        default:
            break;
        }
        std::cout << weOutHere << std::endl;
    } 
}