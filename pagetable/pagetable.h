#pragma once

struct MAP
{
    int index;
    bool validFrame;
};

struct LEVEL
{
    int DepthOfLevel;
    PAGETABLE pageTable;
    LEVEL** NextLevelPtr;
    MAP* MapPtr;

    LEVEL();
    LEVEL(int depth, PAGETABLE* PageTable, std::vector<LEVEL*>* NextLevelPtr);
    LEVEL(int depth, PAGETABLE* PageTable, std::vector<MAP>* MapPtr);
    void PageInsert(PAGETABLE* pageTable, unsigned int LogicalAddress, unsigned int Frame);
};

struct PAGETABLE {
    LEVEL* RootNodePtr; // Contains the address of LEVEL 0

    int levelCount; //number of levels in the system
    std::vector<unsigned int> numberOfBits; //number of bits per level
    std::vector<unsigned int> bitMaskArray; //bit masks per level
    std::vector<unsigned int> shiftArray; //bit shift per level
    std::vector<int> entryCount; // EntryCount[i]: # of possible pages for level i 2^8 for example

    std::vector<MAP> maps;
    std::vector<LEVEL*> nextLevel;

    PAGETABLE(int levCount, std::vector<unsigned int> numOfBits);
    MAP* PageLookup(unsigned int LogicalAddress);
    void PageInsert(unsigned int LogicalAddress, unsigned int Frame);
    unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);
    void LevelMaskCalc(std::vector<unsigned int> bitsPerLev);
    void ShiftAryCalc(std::vector<unsigned int> bitsPerLev);
};

