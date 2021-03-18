#include <iostream>

struct MAP
{
    int index;
    bool validFrame;
};

struct PAGETABLE 
{
    int levelCount;
    int bitNum;
    unsigned int* numberOfBits; //number of bits per level
    unsigned int*bitMaskAry; //bit masks per level
    unsigned int*shiftAry; //bit shift per level
    int *entryCount; //entries per level

    PAGETABLE(int levelCount, int bitNumber)
    {
        this->levelCount = levelCount;
        this->bitNum = bitNumber;
        numberOfBits = new unsigned int[this->levelCount];
        bitMaskAry = new unsigned int[this->levelCount];
        shiftAry = new unsigned int[this->levelCount];
        entryCount = new int[this->levelCount];
    };
};

struct LEVEL//idk
{
    int DepthOfLevel;
    PAGETABLE pageTable;
    LEVEL** NextLevelPtr;
    void PageInsert(PAGETABLE* pageTable, unsigned int LogicalAddress, unsigned int Frame);
};

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift)//idek
{
    unsigned int pageNum;
    return pageNum;
}

MAP* PageLookup(PAGETABLE* PageTable, unsigned int LogicalAddress) 
{

}

void PageInsert() 
{

}