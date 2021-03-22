#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include "pagetable.h"
extern "C"
{
#include "byutr.h"
}
#include "output_mode_helpers.h"


void optionals(OutputOptionsType *input, std::string cmdLineArg) 
{
    input->summary = false;
    if (cmdLineArg.compare("bitmasks")) 
    {input->bitmasks = true;}
    else if (cmdLineArg.compare("logical2physical")) 
    {input->logical2physical = true;}
    else if (cmdLineArg.compare("page2frame"))
    {input->page2frame = true;}
    else if (cmdLineArg.compare("offset"))
    {input->offset = true;}
    else 
    {
        std::cout << "-o argument inputted invalid, please enter valid input" << std::endl; 
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char** argv)
{
    OutputOptionsType arguments;
    arguments.summary = true;
    FILE* inputFile;
    p2AddrTr traceItem;;
    bool complete = false;
    std::vector<unsigned int> levels;
    int memRefLim = 0, argVal = 0, levelNum = 0;/*Captures command line argument values*/

    if (argc < 2)
    {
        std::cout << "Please enter a file" << std::endl;
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
            break;
        }
    }
    for (int i = optind+1; i < argc; i++)
    {
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

    if (memRefLim > 0)
    {
        for (int i = 0; i < memRefLim; i++)
        {
            NextAddress(inputFile, &traceItem);
            std::cout << "Address: " << traceItem.addr << std::endl;
        }
    }
    else
    {
        while (!complete)
        {
            memRefLim++;
            int weThereYet = NextAddress(inputFile, &traceItem);
            complete = (weThereYet == 0);
            if (!complete)
            {
                std::cout << "Address: " << traceItem.addr << std::endl;
            }
        }
    }

    if (arguments.bitmasks) {
        uint32_t* convert;        /*uin32_t array that stores bitmask vector data to be used in report_bitmasks function*/
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