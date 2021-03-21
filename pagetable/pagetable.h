#pragma once

struct MAP
{
    int index;
    bool validFrame;
};

struct PAGETABLE;

struct LEVEL
{
    int DepthOfLevel;   // Which level is this?
    PAGETABLE* PageTablePtr;   // Point to the PageTable that contains the root level node
    std::vector<LEVEL*>* NextLevelPtr;   // Array of pages in this level, each element points to a level node in the next level
    std::vector<MAP>* MapPtr;

    std::vector<LEVEL*> nextLevel;
    std::vector<MAP> maps;

    LEVEL();

    LEVEL(int depth, PAGETABLE* PageTable, int entryCount);

    void PageInsert(PAGETABLE* pageTable, unsigned int LogicalAddress, unsigned int Frame);
};

struct PAGETABLE {
    LEVEL* RootNodePtr; // Contains the address of LEVEL 0

    int levelCount; //number of levels in the system
    std::vector<unsigned int> numberOfBits; //number of bits per level
    std::vector<unsigned int> bitMaskArray; //bit masks per level
    std::vector<unsigned int> shiftArray; //bit shift per level
    std::vector<int> entryCount; // EntryCount[i]: # of possible pages for level i 2^8 for example

    PAGETABLE(int levCount, std::vector<unsigned int> numOfBits);
    MAP* PageLookup(unsigned int LogicalAddress);
    void PageInsert(unsigned int LogicalAddress, unsigned int Frame);
    unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);
    void LevelMaskCalc(std::vector<unsigned int> bitsPerLev);
    void ShiftAryCalc(std::vector<unsigned int> bitsPerLev);
};

