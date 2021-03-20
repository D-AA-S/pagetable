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
    int pathing = 0;

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
            memRefLim = atoi(optarg);
            pathing += 2;
            break;
        case 'o':
            outPutOp = optarg;
            pathing += 2;
            break;
        default:
            std::cout << "wha" << std::endl; 
            break;
        }
    } 
    std::cout << argv[optind] << std::endl;
    return 0; 
}