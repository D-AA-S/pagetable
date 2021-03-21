#pragma once

struct MAP
{
    int index;
    bool validFrame;
};

struct PAGETABLE {
    LEVEL* RootNodePtr;
    int levelCount; //number of levels in the system
    std::vector<unsigned int> numberOfBits; //number of bits per level
    std::vector<unsigned int> bitMaskAry; //bit masks per level
    std::vector<unsigned int> shiftAry; //bit shift per level
    std::vector<int> entryCount; // EntryCount[i]: # of possible pages for level i 2^8 for example

    PAGETABLE(int levCount, std::vector<unsigned int> numOfBits);
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