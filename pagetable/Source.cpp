#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <unistd.h>
#include "pagetable.h"
extern "C" {
#include "byutr.h"
}
#include "output_mode_helpers.h"

/*Reads a file of addresses, and constructs a pagetable of varying sizes based on command line arguments 
Based off of which output mode is selected or summary be default the relavent data will be printed out, onto the console*/
int main(int argc, char** argv)
{
    FILE* inputFile = NULL; //Stores the file argument from the command line 
    uint32_t* convert = NULL; //Dynamic array for converting integer vectors into uint32 integer arrays
    int hits = 0; //number of successful lookup functions
    unsigned int localFrame = 0; //local frame variable for outputting options
    int physMap = 0; //total amount of bits distributed for levels
    p2AddrTr* traceItem = new p2AddrTr(); //Used for the Next Address function
    bool complete = false; //Boolean to track file scanning process
    std::vector<unsigned int> levels; //Stores the amount of bits that each level will use
    int memRefLim = 0, memRefAmt = 0, addressnum = 0,argVal = 0, levelNum = 0;//Captures command line argument values
    uint32_t maskTot = 0; //Only use for outputting logical addresses and their offsets
    //booleans to handle optional arguments
    int summary = true, bitmasks = false, logical2physical = false, offset = false, page2frame = false;

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
            if (atoi(optarg) <= 0) {
                std::cout << "Addresses to scan must be greater than 0" << std::endl;
                exit(EXIT_FAILURE);
            }
            memRefLim = atoi(optarg);
            memRefAmt = memRefLim;
            addressnum = memRefLim;
            break;
        case 'o':
            summary = false;
            if ((std::string)optarg == ("bitmasks"))
                bitmasks = true;
            else if ((std::string)optarg == ("logical2physical"))
                logical2physical = true;
            else if ((std::string)optarg == ("page2frame"))
                page2frame = true;
            else if ((std::string)optarg == ("offset"))
                offset = true;
            else if ((std::string)optarg == ("summary"))
                summary = true;
            else
            {
                std::cout << optarg << " is not a valid -o option, please enter valid input" << std::endl;
                exit(EXIT_FAILURE);
            }
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

    //
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

    PAGETABLE *test = new PAGETABLE(levelNum, levels);
    inputFile = fopen(argv[optind], "r");

    //checks for proper file input
    if (inputFile == NULL)
    {
        std::cout << "File either does not exsist or is unopenable" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Sums the content of the mask array for bitwise for the specified output types
    if (logical2physical || offset)
    {
        maskTot = test->GetMaskTot();
    }
    
    /*iterates through the input file for specified amount of addresses or until reached the end of the file
    * Outputs during the loop if optional arguments were received*/
    while (!complete)
    {
        int scanningProg = NextAddress(inputFile, traceItem); //Used to keep track where NextAddress is in the file
        uint32_t address = traceItem->addr;
        if (test->PageLookup(traceItem->addr) == NULL)
        {
            localFrame = frame;
            test->PageInsert(traceItem->addr, frame);
        }
        else
        {
            localFrame = test->PageLookup(traceItem->addr)->index;
            hits++;
        }

        if (!memRefLim > 0) {
            complete = (scanningProg == 0);
            addressnum++;
        }

        if (!complete)
        {
            if (offset) //if the offset option is set outputs the logical address, and the offset
            {
                uint32_t dest = (address & ~maskTot);
                report_logical2offset(address, (address & ~maskTot));
            }
            else if (logical2physical) //if the logical2physical option is set outputs the address and its physical location
            {
                report_logical2physical(address, test->FramePlusOffSet(address, localFrame, maskTot ,physMap));
            }
            else if (page2frame) //Writes out the page numbers for the address & the frame
            {
                convert = new uint32_t[test->levelCount];
                for (int i = 0; i < test->levelCount; i++) {
                    convert[i] = (uint32_t)(address & (uint32_t)test->GetBitMask()[i]);
                    convert[i] >>= (uint32_t)test->GetShiftArray()[i];
                }
                report_pagemap(address, test->levelCount, convert, localFrame);
            }
        }
        if (memRefLim > 0)
        {
            memRefAmt--;
            complete = (memRefAmt == 0);
        }
    }

    //bitmask output when bitmask optional argument is received
    if (bitmasks) {
        convert = new uint32_t[test->levelCount];
        for (int i = 0; i < test->levelCount; i++)
            convert[i] = (uint32_t) test->GetBitMask().at(i);
        report_bitmasks(test->levelCount, convert);
    }
    //Default output option if no -o optional argument were received
    else if (summary) 
    {
        unsigned int store;
        while (levels.empty()) 
        {
            store += levels.back();
            levels.pop_back();
        }
        unsigned int pagesize = pow(2,(SYSTEMSIZE - physMap));
        report_summary(pagesize, hits, addressnum ,frame, test->ByteCalc(*test, pagesize, addressnum , hits));
    }
    exit(EXIT_SUCCESS);
}