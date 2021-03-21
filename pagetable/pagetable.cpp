#include <iostream>
#include <vector>
#include <cmath>

static int SYSTEMSIZE = 32;

struct MAP
{
    int index;
    bool validFrame;
};

struct PAGETABLE;

struct LEVEL//idk
{
    int DepthOfLevel;   // Which level is this?
    PAGETABLE* PageTablePtr;   // Point to the PageTable that contains the root level node
    std::vector<LEVEL*>* NextLevelPtr;   // Array of pages in this level, each element points to a level node in the next level
    std::vector<MAP>* MapPtr;

    std::vector<LEVEL*> nextLevel;
    std::vector<MAP> maps;

    LEVEL() {

    };

    LEVEL(int depth, PAGETABLE* PageTable, int entryCount)
    {
        DepthOfLevel = depth;
        PageTablePtr = PageTable;
        
        if (PageTablePtr->levelCount == 1) // We don't need nextLevel ptrs. Have it point directly to the Map
        {
            maps.resize(entryCount);
            MapPtr = &maps;
        }
        else // Setup Level 0 with NextLevelPtr pointing to an array of LEVEL pointers
        {
            nextLevel.resize(entryCount);
            NextLevelPtr = &nextLevel;
        }
    };

    void PageInsert(PAGETABLE* pageTable, unsigned int LogicalAddress, unsigned int Frame)
    {

    }
};

struct PAGETABLE 
{
    LEVEL* RootNodePtr; // Contains the address of LEVEL 0

    int levelCount; //number of levels in the system
    std::vector<unsigned int> numberOfBits; //number of bits per level
    std::vector<unsigned int> bitMaskArray; //bit masks per level
    std::vector<unsigned int> shiftArray; //bit shift per level
    std::vector<int> entryCount; // EntryCount[i]: # of possible pages for level i 2^8 for example

    PAGETABLE(int levCount, std::vector<unsigned int> numOfBits)
    {
        levelCount = levCount; //number of levels in the system; For example: 3 levels
        numberOfBits = numOfBits; //number of bits per level; For example: [8, 8, 8]
        LevelMaskCalc(numberOfBits);    // Assigns the bitMaskArray with the appropriate bitmasks per level; For example: [FF000000, 00FF0000, 0000FF00]
        ShiftAryCalc(numberOfBits);     // Assigns the shiftArray with the appropriate bits to shift per level; For example: [24, 16, 8]

        for (int i = 0; i < levelCount; i++)    // Assigns the entryCount vector with the number of possible pages per level
            entryCount[i] = pow(2, numberOfBits[i]);

        // Setup Level 0
        RootNodePtr = &LEVEL(0, this, entryCount[0]); // Assign the address of the newly created LEVEL to the RootNodePtr. This points to Level 0
    };

    MAP* PageLookup(unsigned int LogicalAddress) 
    {

    }

    void PageInsert(unsigned int LogicalAddress, unsigned int Frame) // Used to add new entries to the page table when we have discovered that a page has not yet been allocated(PageLookup returns NULL).
    {

    }
    
    unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift)
    {
        //unsigned int pageNum = 0;
        //return pageNum;
    }

private:
    void LevelMaskCalc(std::vector<unsigned int> bitsPerLev)
    {
        unsigned int tempBitMask = 0b0;
        int shift = 0;
        for (int i = 0; i < bitsPerLev.size(); i++) 
        {
            for (int j = bitsPerLev[i]; j > 0; j--) {
                tempBitMask | 0b1;
                tempBitMask << 1;
            }
            tempBitMask << SYSTEMSIZE - bitsPerLev[i] - shift;
            shift += bitsPerLev[i];
            bitMaskArray[i] = tempBitMask;
        }
    }

    void ShiftAryCalc(std::vector<unsigned int> bitsPerLev)
    {
        int fuck = 0;
        for (int i = 0; i < bitsPerLev.size(); i++) 
        {
            shiftArray[i] = SYSTEMSIZE - bitsPerLev[i] - fuck;
            fuck += bitsPerLev[i];
        }
    }
};


