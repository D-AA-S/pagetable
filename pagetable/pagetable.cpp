#include <iostream>
#include <vector>
#include <cmath>

#include "pagetable.h"
#include <bitset>

const int SYSTEMSIZE = 32;
unsigned int frame = 0;

MAP::MAP()
{
    index = -1;
    validFrame = false;
}

MAP::MAP(int index, bool validFrame)
{
    this->index = index;
    this->validFrame = validFrame;
}

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
    unsigned int pageIndex = LogicalToPage(LogicalAddress, bitMask, shift);

    if (currentDepth == levelCount - 1)   // If the current depth is equal to the levelCount - 1, then we are at a leaf node
    {
        if (levelPtr->MapPtr == NULL)   //There are no maps for this address yet; create a new map array
            levelPtr->MapPtr = new std::vector<MAP>(entryCount);
        if (levelPtr->MapPtr->at(pageIndex).validFrame == true)    // there already exists a mapping here. Do not need to replace
            return;
        MAP* newMap = new MAP(Frame, true);
        levelPtr->MapPtr->at(pageIndex) = *newMap;
        frame++;
    }
    else
    {
        if (levelPtr->NextLevelPtr == NULL)
            levelPtr->NextLevelPtr = new std::vector<LEVEL*>(entryCount);
        if (levelPtr->NextLevelPtr->at(pageIndex) == NULL)
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
    MAP* map = new MAP();
    unsigned int currentDepth;
    LEVEL* current = RootNodePtr;

    for (int i = 0; i < levelCount; i++)
    {
        currentDepth = current->DepthOfLevel;
        unsigned int page = LogicalToPage(LogicalAddress, bitMaskArray[i], shiftArray[i]);
        if (currentDepth == levelCount - 1) // We are at the maps. Check to see if the map exisits. If it doesn't, return null. If it does, return the map!
        {
            if (current->MapPtr == NULL)
                return NULL;
            else if (current->MapPtr->at(page).validFrame == false)
                return NULL;
            else
            {
                map = &(current->MapPtr->at(page));
                return map;
            }
        }
        else
        {
            if (current->NextLevelPtr == NULL)
                return NULL;
            else
                current = current->NextLevelPtr->at(page);
        }
    }
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
        tempBitMask <<= SYSTEMSIZE - bitsPerLev[i] - shift;
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
        maskTot |= bitMaskArray[i];
    }
    return maskTot;
}

std::vector<unsigned int> &PAGETABLE::GetBitMask()
{
    return bitMaskArray;
}

std::vector<unsigned int> &PAGETABLE::GetShiftArray() 
{
    return shiftArray;
}

std::vector<unsigned int> &PAGETABLE::GetNumberOfBits()
{
    return numberOfBits;
}

uint32_t PAGETABLE::FramePlusOffSet(uint32_t address, uint32_t frame, uint32_t mask, unsigned int physmap)
{
    address = address & ~mask;
    frame<<=(SYSTEMSIZE-physmap);
    address = frame +address;
    return address; 
}

unsigned int PAGETABLE::ByteCalc()
{
    unsigned int byteTot = 0;
    byteTot += sizeof(levelCount);
    return byteTot;
}