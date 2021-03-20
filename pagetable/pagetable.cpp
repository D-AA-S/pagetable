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
    unsigned int *numberOfBits; //number of bits per level
    unsigned int *bitMaskAry; //bit masks per level
    unsigned int *shiftAry; //bit shift per level
    int *entryCount; // EntryCount[i]: # of possible pages for level i 2^8 for example

    PAGETABLE(int levelCount, unsigned int *numberOfBits)
    {
        this->levelCount = levelCount; //number of levels in the system; For example: 3 levels
        this->numberOfBits = numberOfBits; //number of bits per level; For example: [8, 8, 8]
        bitMaskAry = new unsigned int[this->levelCount]; //bit masks per level; This will set up an empty array of size levelCount that will hold unsigned ints
        bitMaskAry = LevelMaskCalc(this->numberOfBits);
        shiftAry = new unsigned int[this->levelCount]; // this will hold the bit shifts per level; For example: [24, 16, 8]
        ShiftAryCalc(this->numberOfBits, shiftAry);
        entryCount = new int[this->levelCount];

        for (int i = 0; i < levelCount; i++)
            entryCount[i] = pow(2, numberOfBits[i]);

    };

    MAP* PageLookup(unsigned int LogicalAddress) 
    {

    }

    void PageInsert(unsigned int LogicalAddress, unsigned int Frame) // Used to add new entries to the page table when we have discovered that a page has not yet been allocated(PageLookup returns NULL).
    {

    }
    
    unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift)
    {
        unsigned int pageNum = 0;
        return pageNum;
    }

private:
    unsigned int* LevelMaskCalc(unsigned int *bitsPerLev) 
    {
        unsigned int tempBitMask = 0b0;
        int shift = 0;
        for (int i = 0; i < (sizeof(bitsPerLev)/bitsPerLev[0]); i++) 
        {
            for (int j = bitsPerLev[i]; j > 0; j--) {
                tempBitMask | 0b1;
                tempBitMask << 1;
            }
            tempBitMask << SYSTEMSIZE - bitsPerLev[i] - shift;
            shift += bitsPerLev[i];
            bitMaskAry[i] = tempBitMask;
        }
        return bitMaskAry;
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


