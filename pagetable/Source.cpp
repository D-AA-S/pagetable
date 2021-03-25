#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include "pagetable.h"
extern "C"
{
#include "byutr.h"
}
#include "output_mode_helpers.h"

/*Receives optional argument from main function, and converts cmdLineArg to lowercase only
to accurately parse what optional argument is being inputted*/
void optionals(OutputOptionsType *input, std::string cmdLineArg) 
{
    std::transform(cmdLineArg.begin(), cmdLineArg.end(), cmdLineArg.begin(), ::tolower);
    input->summary = false, input->bitmasks = false, input->logical2physical = false, 
        input->offset = false, input->page2frame = false;
    if (cmdLineArg == ("bitmasks"))
    {
        input->bitmasks = true;
    }
    else if (cmdLineArg == ("logical2physical"))
    {
        input->logical2physical = true;
    }
    else if (cmdLineArg == ("page2frame"))
    {
        input->page2frame = true;
    }
    else if (cmdLineArg == ("offset"))
    {
        input->offset = true;
    }
    else 
    {
        std::cout << "-o argument inputted invalid, please enter valid input" << std::endl; 
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char** argv)
{
    OutputOptionsType arguments; //Structure that keeps track of optional arguments
    FILE* inputFile; //Stores the file argument from the command line
    uint32_t* convert;
    int hits = 0;
    unsigned int localFrame;
    unsigned int physMap;
    p2AddrTr traceItem; //Used for the Next Address function
    bool complete = false; //Boolean to track file scanning process
    std::vector<unsigned int> levels; //Stores the amount of bits that each level will use
    int memRefLim = 0, memRefAmt = 0, argVal = 0, levelNum = 0;//Captures command line argument values
    uint32_t maskTot; //Only use for outputting logical addresses and their offsets

    if (argc < 3)
    {
        std::cout << "Please enter a file & amount of bits for single/multiple levels" << std::endl;
        exit(EXIT_FAILURE);
    }
    while ((argVal = getopt(argc, argv, "n:o:")) != -1)
    {
        switch (argVal)
        {
        case 'n':
            memRefLim = atoi(optarg);
            memRefAmt = memRefLim;
            break;
        case 'o':
            optionals(&arguments,optarg);
            break;
        default:
            std::cout << "Invalid optional argument" << std::endl;
            exit(EXIT_FAILURE);
            break;
        }
    }

    for (int i = optind+1; i < argc; i++)
    {
        if (atoi(argv[i]) >= SYSTEMSIZE) 
        {
            std::cout << "The amount of bits allocated is greater than the alloted maximum" << std::endl;
            exit(EXIT_FAILURE);
        }
        levels.push_back(atoi(argv[i]));
        physMap = arguments.logical2physical ? +atoi(argv[i])+physMap : 0;
        levelNum++;
    }
    PAGETABLE test(levelNum, levels);
    inputFile = fopen(argv[optind], "r");
    if (inputFile == NULL)
    {
        std::cout << "File either does not exsist or is unopenable" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (arguments.page2frame) 
    {
        uint32_t* convert; //uin32_t array that stores the bits per bits per level vector to be used in report_pagemap
        convert = new uint32_t[test.GetNumberOfBits().size()];
        for (int i = 0; i < test.GetNumberOfBits().size(); i++)
            convert[i] = (uint32_t)test.GetNumberOfBits().at(i);
    }

    if (arguments.bitmasks, arguments.logical2physical) 
    {
        maskTot = test.GetMaskTot();
    }


    while (!complete)
    {
        int scanningProg = NextAddress(inputFile, &traceItem); //Used to keep track where NextAddress is in the file
        if (!test.PageLookup(traceItem.addr))
        {
            localFrame = frame;
            test.PageInsert(traceItem.addr, frame);
        }
        else
        {
            localFrame = test.PageLookup(traceItem.addr)->index;
            hits++;
        }
        if (memRefLim > 0) 
        {
            memRefAmt--;
            complete = (memRefAmt == 0);
        }
        else
            complete = (scanningProg == 0);
        if (!complete)
        {
            if (arguments.offset)
            {
                report_logical2offset(traceItem.addr, (traceItem.addr & ~maskTot));
            }
            else if (arguments.logical2physical)
            {
                report_logical2physical(traceItem.addr, test.FramePlusOffSet(traceItem.addr, localFrame, maskTot ,physMap));
            }
            else if (arguments.page2frame)
            {
                report_pagemap(traceItem.addr, test.levelCount, convert, localFrame);
            }
        }
    }

    if (arguments.bitmasks) {
        convert = new uint32_t[test.GetBitMask().size()];
        for (int i = 0; i < test.GetBitMask().size(); i++)
            convert[i] = (uint32_t)test.GetBitMask().at(i);
        report_bitmasks(test.levelCount, convert);
    }
    else if (arguments.summary) 
    {
        unsigned int store;
        while (levels.empty()) 
        {
            store = levels.back();
            levels.pop_back();
        }
        unsigned int pagesize = 2^(SYSTEMSIZE - store);
        //report_summary(pagesize, /*hits variable*/, memRefLim,frame,);
    }
    exit(EXIT_SUCCESS);
}