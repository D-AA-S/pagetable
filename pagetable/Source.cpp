#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include "pagetable.h"
#include "byutr.h"
#include "output_mode_helpers.h"

int main(int argc, char** argv)
{
    FILE* inputFile;
    p2AddrTr traceItem;;
    bool complete = false;
    std::vector<unsigned int> levels;
    int levelNum = 0;
    std::string outPutOp = "";
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
    for (int i = optind + 1; i < argc; i++)
    {
        levels.push_back(atoi(argv[i]));
    }
    //PAGETABLE *test = new PAGETABLE(levelNum, levels);
    inputFile = fopen(argv[optind], "r");
    if (inputFile = NULL)
    {
        std::cout << "File either does not exsist or is unopenable" << std::endl;
        return -1;
    }

    if (memRefLim > 0)
    {
        for (int i = 0; i < memRefLim; i++)
        {

        }
    }
    else
    {
        while (!complete)
        {
            int weThereYet = NextAddress(inputFile, &traceItem);
            complete = (weThereYet == 0);
            if (!complete)
            {
                std::cout << "Address: " << traceItem.addr << std::endl;
            }
        }
    }

    if (outPutOp != "")
    {

    }
    return 0;
}