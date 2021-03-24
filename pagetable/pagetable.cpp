#include <iostream>
#include <vector>
#include <cmath>

#include "pagetable.h"
#include <bitset>

const int SYSTEMSIZE = 32;
unsigned int frame = 0;

MAP::MAP()
{

}

MAP::MAP(int index, bool validFrame)
{
    this->index = index;
    this->validFrame = validFrame;
}

LEVEL::LEVEL()
{

};

LEVEL::LEVEL(int depth, PAGETABLE& PageTable)
{
    DepthOfLevel = depth;
    PageTablePtr = &PageTable;
};

void PAGETABLE::PageInsert(unsigned int LogicalAddress, unsigned int Frame) // Used to add new entries to the page table when we have discovered that a page has not yet been allocated(PageLookup returns NULL).
{
    PageInsert(RootNodePtr, LogicalAddress, Frame);
}

void PAGETABLE::PageInsert(LEVEL* levelPtr, unsigned int LogicalAddress, unsigned int Frame)
{
    unsigned int currentDepth = levelPtr->DepthOfLevel;
    unsigned int bitMask = levelPtr->PageTablePtr->bitMaskArray[currentDepth];  //Get the bitmask for the current level
    unsigned int entryCount = levelPtr->PageTablePtr->entryCount[currentDepth];
    unsigned int shift = levelPtr->PageTablePtr->shiftArray[currentDepth];
    std::vector<MAP>* mapping = new std::vector<MAP>;
    std::vector<LEVEL*>* levels = new std::vector<LEVEL*>;

    if (currentDepth == levelPtr->PageTablePtr->levelCount - 1)   // If the current depth is equal to the levelCount - 1, then we are at a leaf node
    {
        if (levelPtr->MapPtr == 0)   //There are no maps for this address yet; create a new map array
        {
            mapping->resize(entryCount);
            levelPtr->MapPtr = mapping;
        }
        unsigned int pageIndex = LogicalToPage(LogicalAddress, bitMask, shift);
        if (levelPtr->MapPtr->at(pageIndex).validFrame == true)    // there already exists a mapping here. Do not need to replace
            return;
        MAP* newMap = new MAP(Frame, true);
        levelPtr->MapPtr->at(pageIndex) = *newMap;
    }
    else
    {
        if (levelPtr->NextLevelPtr == 0)
        {
            levels->resize(entryCount);
            levelPtr->NextLevelPtr = levels;
        }
        //LEVEL nextLevel(currentDepth + 1, *this);
        unsigned int pageIndex = LogicalToPage(LogicalAddress, bitMask, shift);
        //levelPtr->NextLevelPtr->at(pageIndex) = &nextLevel;
        if (levelPtr->NextLevelPtr->at(pageIndex) == 0)
            levelPtr->NextLevelPtr->at(pageIndex) = new LEVEL(currentDepth + 1, *this);
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

    RootNodePtr = new LEVEL(0, *this); // Assign the address of the newly created LEVEL to the RootNodePtr. This points to Level 0
};

MAP* PAGETABLE::PageLookup(unsigned int LogicalAddress) //need to finish this function - not done yet
{
    MAP testermap(3, true);
    MAP* map = &testermap;
    LEVEL* current = RootNodePtr;   // Need to finish writing this for loop to look up the pages
    for (int i = 0; i < levelCount; i++)
    {
        unsigned int page = LogicalToPage(LogicalAddress, bitMaskArray[i], shiftArray[i]);
        current->NextLevelPtr->at(page);
    }
    return map;
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

uint32_t PAGETABLE::GetMaskTot()
{
    uint32_t maskTot = 0;
    for (int i = 0; i < bitMaskArray.size(); i++)
    {
        maskTot += bitMaskArray.at(i);
    }
    return maskTot;
}

std::vector<unsigned int> PAGETABLE::GetBitMask()
{
    return bitMaskArray;
}

std::vector<unsigned int> PAGETABLE::GetNumberOfBits()
{
    return numberOfBits;
}

uint32_t PAGETABLE::framePlusOffSet(uint32_t address, uint32_t *frame, uint32_t *mask)
{
    address = address & ~*mask;
    address += (*frame << SYSTEMSIZE - *mask);
}