#include <iostream>
#include <vector>
#include <cmath>

#include "pagetable.h"
#include <bitset>

static int SYSTEMSIZE = 32;

LEVEL::LEVEL()
{

};

LEVEL::LEVEL(int depth, PAGETABLE* PageTable, int entryCount)
{
    DepthOfLevel = depth;
    PageTablePtr = PageTable;

    if (PageTablePtr->levelCount == 1) // We don't need nextLevel ptrs. Have it point directly to the Map
    {
        maps.resize(entryCount);
        MapPtr = &maps;
    }
    else // Setup Level 0 with NextLevelPtr pointing to an array of LEVEL pointers
    {
        nextLevel.resize(entryCount);
        NextLevelPtr = &nextLevel;
    }
};

void LEVEL::PageInsert(LEVEL* levelPtr, unsigned int LogicalAddress, unsigned int Frame)
{

}

PAGETABLE::PAGETABLE(int levCount, std::vector<unsigned int> numOfBits)
{
    levelCount = levCount; //number of levels in the system; For example: 3 levels
    numberOfBits = numOfBits; //number of bits per level; For example: [8, 8, 8]
    LevelMaskCalc(numberOfBits);    // Assigns the bitMaskArray with the appropriate bitmasks per level; For example: [FF000000, 00FF0000, 0000FF00]
    ShiftAryCalc(numberOfBits);     // Assigns the shiftArray with the appropriate bits to shift per level; For example: [24, 16, 8]

    for (int i = 0; i < levelCount; i++)    // Assigns the entryCount vector with the number of possible pages per level
        entryCount.push_back(pow(2, numberOfBits[i]));

    // Setup Level 0

    LEVEL root(0, this, entryCount[0]);
    RootNodePtr = &root; // Assign the address of the newly created LEVEL to the RootNodePtr. This points to Level 0
};

MAP* PAGETABLE::PageLookup(unsigned int LogicalAddress)
{
    MAP* map;
    LEVEL* current = RootNodePtr;   // Need to finish writing this for loop to look up the pages
    for (int i = 0; i < levelCount; i++)
    {
        unsigned int page = LogicalToPage(LogicalAddress, bitMaskArray[i], shiftArray[i]);
        current->NextLevelPtr->at(page);
    }
}


void PAGETABLE::PageInsert(unsigned int LogicalAddress, unsigned int Frame) // Used to add new entries to the page table when we have discovered that a page has not yet been allocated(PageLookup returns NULL).
{
    return;
}

unsigned int PAGETABLE::LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift)
{
    unsigned int page = LogicalAddress & Mask;
    page >>= Shift;
    return page;    // returns an unsigned int containing the pageNumber. Check to see if a LEVEL exists in the array of LEVEL pointers
}

void PAGETABLE::LevelMaskCalc(std::vector<unsigned int> bitsPerLev)
{
    unsigned int tempBitMask = 0b0;
    unsigned int temp;
    int shift = 0;
    for (int i = 0; i < bitsPerLev.size(); i++)
    {
        for (int j = bitsPerLev[i]; j > 0; j--) {
            tempBitMask <<= 1;
            tempBitMask |= 0b1;
        }
        //std::cout << std::bitset<32>(tempBitMask) << std::endl;     //Just testing to make sure the bitmask is set properly
        tempBitMask <<= SYSTEMSIZE - bitsPerLev[i] - shift;
        //std::cout << std::bitset<32>(tempBitMask) << std::endl;
        shift += bitsPerLev[i];
        bitMaskArray.push_back(tempBitMask);
        tempBitMask = 0;
    }
}

void PAGETABLE::ShiftAryCalc(std::vector<unsigned int> bitsPerLev)
{
    int shift = 0;
    for (int i = 0; i < bitsPerLev.size(); i++)
    {
        shiftArray.push_back(SYSTEMSIZE - bitsPerLev[i] - shift);
        shift += bitsPerLev[i];
    }
}

std::vector<unsigned int> PAGETABLE::GetBitMask() 
{
    return bitMaskArray;
}
