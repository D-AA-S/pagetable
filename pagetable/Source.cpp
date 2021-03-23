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
    p2AddrTr traceItem; //Used for the Next Address function
    bool complete = false; //Boolean to track file scanning process
    std::vector<unsigned int> levels; //Stores the amount of bits that each level will use
    int memRefLim = 0, argVal = 0, levelNum = 0;//Captures command line argument values
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
        levelNum++;
    }
    PAGETABLE test(levelNum, levels);
    inputFile = fopen(argv[optind], "r");
    if (inputFile == NULL)
    {
        std::cout << "File either does not exsist or is unopenable" << std::endl;
        exit(EXIT_FAILURE);
    }

    maskTot = test.GetMaskTot();
    if (memRefLim > 0)
    {
        for (int i = 0; i < memRefLim; i++)
        {
            NextAddress(inputFile, &traceItem);
            if (arguments.offset) 
            {
                report_logical2offset(traceItem.addr, (traceItem.addr & ~maskTot));
            }
        }
    }
    else
    {
        while (!complete)
        {
            memRefLim++;
            int scanningProg = NextAddress(inputFile, &traceItem); //Used to keep track of how many addresses have been scanned
            complete = (scanningProg == 0);
            if (!complete)
            {
                if (arguments.offset)
                {
                    report_logical2offset(traceItem.addr, (traceItem.addr & ~maskTot));
                }
            }
        }
    }

    if (arguments.bitmasks) {
        uint32_t* convert;        //uin32_t array that stores bitmask vector data to be used in report_bitmasks function
        convert = new uint32_t[test.GetBitMask().size()];
        for (int i = 0; i < test.GetBitMask().size(); i++)
        {
            convert[i] = (uint32_t)test.GetBitMask().at(i);
        }
        report_bitmasks(test.levelCount, convert);
    }
    /*case 'L':
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
    }*/
    exit(EXIT_SUCCESS);
}