#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include "pagetable.h"
extern "C"
{
#include "byutr.h"
#include "output_mode_helpers.h"
}

int main(int argc, char** argv)
{
    FILE* inputFile;
    p2AddrTr traceItem;;
    bool complete = false;
    std::vector<unsigned int> levels;
    int levelNum = 0;
    char outPutOp = 'S';
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
            outPutOp = toupper(optarg[0]);
            pathing += 2;
            break;
        default:
            std::cout << "wha" << std::endl;
            break;
        }
    }
    for (int i = optind + 1; i < argc; i++)
    {
        std::cout << argv[i] << std::endl; 
        //levels.push_back(atoi(argv[i]));
    }
    
    PAGETABLE test(levelNum, levels);
    inputFile = fopen(argv[optind], "r");
    if (inputFile = NULL)
    {
        std::cout << "File either does not exsist or is unopenable" << std::endl;
        return -1;
    }
    else
        std::cout << "file is openable" << std::endl; 

    if (memRefLim > 0)
    {
        for (int i = 0; i < memRefLim; i++)
        {
            //NextAddress(inputFile, &traceItem);
            //std::cout << ("Address %d:",i+1) << traceItem.addr << std::endl;
        }
    }
    else
    {
        /*while (!complete)
        {
            int weThereYet = NextAddress(inputFile, &traceItem);
            complete = (weThereYet == 0);
            if (!complete)
            {
                std::cout << "Address: " << traceItem.addr << std::endl;
            }
        }*/
    }

    switch (outPutOp)
    {
    case 'B':
        report_bitmasks(test.levelCount, test.bitMaskArray);
        break;
    case 'L':
        //code for Logical2physical option
        break;
    case 'P':
        //code for page2frame option
        break;
    case 'O':
        //code for offest option
        break;
    default:
        //code for summary when nothing is selected
        break;
    }
    return 0;
}