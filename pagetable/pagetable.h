#pragma once

struct MAP
{
    int index;
    bool validFrame;
};
struct PAGETABLE {
    int levelCount;
    unsigned int* numberOfBits;
    unsigned int* bitMaskAry;
    unsigned int* shiftAry;
    int* entryCount;
    PAGETABLE(int levelCount, unsigned int* numberOfBits);
};
struct LEVEL//idk
{
    int DepthOfLevel;
    PAGETABLE pageTable;
    LEVEL** NextLevelPtr;
    void PageInsert(PAGETABLE* pageTable, unsigned int LogicalAddress, unsigned int Frame);
};