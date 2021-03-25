#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include "pagetable.h"
extern "C" {
#include "byutr.h"
}
#include "output_mode_helpers.h"

/*Receives optional argument from main function, and converts cmdLineArg to lowercase only
to accurately parse what optional argument is being inputted*/
void optionals(OutputOptionsType *input, std::string cmdLineArg) 
{
    std::transform(cmdLineArg.begin(), cmdLineArg.end(), cmdLineArg.begin(), ::tolower);
    input->summary = false;
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
    uint32_t* convert; //Dynamic array for converting integer vectors into uint32 integer arrays
    int hits = 0; //number of successful lookup functions
    unsigned int localFrame; //local frame variable for outputting options
    int physMap = 0; //total amount of bits distributed for levels
    p2AddrTr traceItem; //Used for the Next Address function
    bool complete = false; //Boolean to track file scanning process
    std::vector<unsigned int> levels; //Stores the amount of bits that each level will use
    int memRefLim = 0, memRefAmt = 0, argVal = 0, levelNum = 0;//Captures command line argument values
    uint32_t maskTot = 0; //Only use for outputting logical addresses and their offsets
    arguments.summary = true, arguments.bitmasks = false, arguments.logical2physical = false,
        arguments.offset = false, arguments.page2frame = false;

    //Checks for correct amount of command line arguments
    if (argc < 3)
    {
        std::cout << "Please enter a file & amount of bits for single/multiple levels" << std::endl;
        exit(EXIT_FAILURE);
    }
    //detects and processes optional arguments
    while ((argVal = getopt(argc, argv, "n:o:")) != -1)
    {
        switch (argVal)
        {
        case 'n':
            memRefLim = atoi(optarg);
            memRefAmt = memRefLim;
            break;
        case 'o':
            optionals(&arguments, optarg);
            break;
        default:
            std::cout << "Invalid optional argument" << std::endl;
            exit(EXIT_FAILURE);
            break;
        }
    }
    //checks for atleast 1 level of bits after the file argument
    if (optind+1 == argc) 
    {
        std::cout << "No argument have been imputted for the bits per level/levels" << std::endl; 
        exit(EXIT_FAILURE);
    }
    for (int i = optind + 1; i < argc; i++)
    {
        levels.push_back(atoi(argv[i]));
        physMap = atoi(argv[i]) + physMap;
        if (atoi(argv[i]) >= SYSTEMSIZE || physMap >= SYSTEMSIZE)
        {
            std::cout << "The amount of bits allocated is greater than the alloted maximum" << std::endl;
            exit(EXIT_FAILURE);
        }
        levelNum++;
    }

    PAGETABLE test(levelNum, levels);
    inputFile = fopen(argv[optind], "r");

    //checks for proper file input
    if (inputFile == NULL)
    {
        std::cout << "File either does not exsist or is unopenable" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Sums the content of the mask array for bitwise for the specified output types
    if (arguments.logical2physical || arguments.offset)
    {
        maskTot = test.GetMaskTot();
    }
    
    //iterates through the input file for specified amount of addresses or until reached the end of the file
    //Outputs during the loop if optional arguments were received
    while (!complete)
    {
        int scanningProg = NextAddress(inputFile, &traceItem); //Used to keep track where NextAddress is in the file
        uint32_t address = traceItem.addr;
        if (!test.PageLookup(address))
        {
            localFrame = frame;
            test.PageInsert(address, frame);
        }
        else
        {
            localFrame = test.PageLookup(address)->index;
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
                uint32_t dest = (traceItem.addr & ~maskTot);
                report_logical2offset(address, (address & ~maskTot));
            }
            else if (arguments.logical2physical)
            {
                report_logical2physical(address, test.FramePlusOffSet(address, localFrame, maskTot ,physMap));
            }
            else if (arguments.page2frame)
            {
                convert = new uint32_t[test.levelCount];
                for (int i = 0; i < test.levelCount; i++) {
                    convert[i] = (uint32_t)(address & (uint32_t)test.GetBitMask()[i]);
                    convert[i] >>= (uint32_t)test.GetShiftArray()[i];
                }
                report_pagemap(traceItem.addr, test.levelCount, convert, localFrame);
            }
        }
    }

    //bitmask output when bitmask optional argument is received
    if (arguments.bitmasks) {
        convert = new uint32_t[test.levelCount];
        for (int i = 0; i < test.levelCount; i++)
            convert[i] = (uint32_t)test.GetBitMask().at(i);
        report_bitmasks(test.levelCount, convert);
    }
    //Default output option if no -o optional argument were received
    else if (arguments.summary) 
    {
        unsigned int store;
        while (levels.empty()) 
        {
            store += levels.back();
            levels.pop_back();
        }
        unsigned int pagesize = pow(2,(SYSTEMSIZE - physMap));
        report_summary(pagesize, hits, memRefLim,frame, 32);
    }

    exit(EXIT_SUCCESS);
}