#include <iostream>


struct MAP
{
    int index;
    bool validFrame;
};

struct PAGETABLE 
{
    int levelCount;
    struct mask *bitMaskAry;
    struct shift *shiftAry;
    struct count *entryCount;
};

struct  LEVEL
{
    int DepthOfLevel;
    PAGETABLE pageTable;
    LEVEL** NextLevelPtr;
    void PageInsert(PAGETABLE* pageTable, unsigned int LogicalAddress, unsigned int Frame);
};

int main()
{
    std::cout << "Hello World!\n";
}

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift) 
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