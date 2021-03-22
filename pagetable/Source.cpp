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

int main(int argc, char** argv)
{
    FILE* inputFile;
    p2AddrTr traceItem;;
    bool complete = false;
    std::vector<unsigned int> levels;
    char outPutOp = 'S'; /*Captures what optional output argument is received*/
    int memRefLim = 0, argVal = 0, levelNum = 0;/*Captures command line argument values*/

    if (argc < 2)
    {
        std::cout << "Please enter a file" << std::endl;
        return -1;
    }
    while ((argVal = getopt(argc, argv, "n:o:")) != -1)
    {
        switch (argVal)
        {
        case 'n':
            memRefLim = atoi(optarg);
            break;
        case 'o':
            outPutOp = toupper(optarg[0]);
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
        return -1;
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
            int weThereYet = NextAddress(inputFile, &traceItem);
            complete = (weThereYet == 0);
            if (!complete)
            {
                std::cout << "Address: " << traceItem.addr << std::endl;
            }
        }
    }

    switch (outPutOp)
    {
    case 'B':
        uint32_t * convert;        /*uin32_t array that stores bitmask vector data to be used in report_bitmasks function*/
        convert = new uint32_t[test.GetBitMask().size()];
        for (int i = 0; i < test.GetBitMask().size();i++) 
        {convert[i] = (uint32_t)test.GetBitMask().at(i);}
        report_bitmasks(test.levelCount, convert);
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