#include <iostream>
#include <vector>
#include <cmath>

#include "pagetable.h"
#include <bitset>

static int SYSTEMSIZE = 32;

MAP::MAP(int index, bool validFrame)
{
    this->index = index;
    this->validFrame = validFrame;
}

LEVEL::LEVEL()
{

};

LEVEL::LEVEL(int depth, PAGETABLE* PageTable)
{
    DepthOfLevel = depth;
    PageTablePtr = PageTable; 

    /*
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
    */
};

void PAGETABLE::PageInsert(LEVEL* levelPtr, unsigned int LogicalAddress, unsigned int Frame)
{
    unsigned int currentDepth = levelPtr->DepthOfLevel;
    unsigned int bitMask = levelPtr->PageTablePtr->bitMaskArray[currentDepth];  //Get the bitmask for the current level
    unsigned int entryCount = levelPtr->PageTablePtr->entryCount[currentDepth];
    unsigned int shift = levelPtr->PageTablePtr->shiftArray[currentDepth];
    std::vector<MAP> mapping;
    std::vector<LEVEL*> levels;

    if (currentDepth = levelPtr->PageTablePtr->levelCount - 1)   // If the current depth is equal to the levelCount - 1, then we are at a leaf node
    {
        if (levelPtr->MapPtr == 0)   //There are no maps for this address yet; create a new map array
        {
            mapping.resize(entryCount);
            levelPtr->MapPtr = &mapping;
        }
        unsigned int pageIndex = LogicalToPage(LogicalAddress, bitMask, shift);
        levelPtr->MapPtr->at(pageIndex) = MAP(Frame, true);
    }
    else
    {
        if (levelPtr->NextLevelPtr == 0)
        {
            levels.resize(entryCount);
            levelPtr->NextLevelPtr = &levels;
        }
        LEVEL nextLevel(currentDepth + 1, this);
        unsigned int pageIndex = LogicalToPage(LogicalAddress, bitMask, shift);
        levelPtr->NextLevelPtr->at(pageIndex) = &nextLevel;
        LEVEL* newLevel = levelPtr->NextLevelPtr->at(pageIndex);
        PageInsert(newLevel, LogicalAddress, Frame);
    }
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

    LEVEL root(0, this);
    RootNodePtr = &root; // Assign the address of the newly created LEVEL to the RootNodePtr. This points to Level 0
};

MAP* PAGETABLE::PageLookup(unsigned int LogicalAddress) //need to finish this function - not done yet
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
    PageInsert(this->RootNodePtr, LogicalAddress, Frame);
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
