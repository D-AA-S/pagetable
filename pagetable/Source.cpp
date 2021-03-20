#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pagetable.h"

int main(int argc, char** argv)
{
    std::string outPutOp;
    int memRefLim;
    int weOutHere = 0;
    int loop = 1;

    if (argc < 2) 
    {
        std::cout << "Please enter a file" << std::endl;
        return -1;
    }
    while ((weOutHere = getopt(argc, argv, "n:o:")) != -1)
    {
        switch (weOutHere)
        {
        case 'n':
            memRefLim = atoi(argv[loop]);
            std::cout << memRefLim << std::endl;
            break;
        case 'o':
            outPutOp = argv[loop];
            std::cout << outPutOp << std::endl;
            break;
        default:
            std::cout << "wha" << std::endl; 
            break;
        }
    } 
    return 0; 
}