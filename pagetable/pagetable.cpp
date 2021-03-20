#include <iostream>
#include <cmath>

static int SYSTEMSIZE = 32;

struct MAP
{
    int index;
    bool validFrame;
};

struct PAGETABLE 
{
    int levelCount; //number of levels in the system
    int bitNum; //number of bits in an address
    unsigned int *numberOfBits; //number of bits per level
    unsigned int *bitMaskAry; //bit masks per level
    unsigned int *shiftAry; //bit shift per level
    int *entryCount; //entries per level

    PAGETABLE(int levelCount, int bitNumber, unsigned int *levelBits)
    {
        this->levelCount = levelCount;
        this->bitNum = bitNumber;
        numberOfBits = levelBits;
        bitMaskAry = new unsigned int[this->levelCount];
        shiftAry = new unsigned int[this->levelCount];
        ShiftAryCalc(numberOfBits,shiftAry);
        entryCount = new int[this->levelCount];
    };

    MAP* PageLookup(unsigned int LogicalAddress) 
    {

    }

    void PageInsert(unsigned int LogicalAddress, unsigned int Frame) //Used to add new entries to the page table when we have discovered that a page has not yet been allocated(PageLookup returns NULL).
    {

    }

private:
    unsigned int* LevelMaskCalc(unsigned int *bitsPerLev) 
    {
        unsigned int tempBitMask = 0x00000000;
        for (int i = 0; i < (sizeof(bitsPerLev)/bitsPerLev[0]); i++) 
        {
            for (int j = (bitsPerLev[i] / 4); j > 0; j--) {
                tempBitMask | 0xF;
                tempBitMask << 4;
            }
            tempBitMask |= 
            bitMaskAry[i] = 
        }
    }

    unsigned int* ShiftAryCalc(unsigned int* bitsPerLev, unsigned int* newShiftAry)
    {
        int fuck = 0;
        for (int i = 0; i < sizeof(bitsPerLev)/bitsPerLev[0]; i++) 
        {
            newShiftAry[i] = SYSTEMSIZE - bitsPerLev[i] - fuck;
            fuck += bitsPerLev[i];
        }
        return newShiftAry;
    }
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
    unsigned int pageNum = 0;
    return pageNum;
}
