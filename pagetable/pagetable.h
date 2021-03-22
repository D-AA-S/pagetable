#pragma once

class MAP
{
    int index;
    bool validFrame;
};

class PAGETABLE;
class LEVEL;

class LEVEL
{
private:
    int DepthOfLevel;   // Which level is this?
    PAGETABLE* PageTablePtr;   // Point to the PageTable that contains the root level node
    std::vector<LEVEL*>* NextLevelPtr;   // Array of pages in this level, each element points to a level node in the next level
    std::vector<MAP>* MapPtr;
    std::vector<LEVEL*> nextLevel;
    std::vector<MAP> maps;

public:
    LEVEL();
    LEVEL(int depth, PAGETABLE* PageTable, int entryCount);
    void PageInsert(PAGETABLE* pageTable, unsigned int LogicalAddress, unsigned int Frame);
};

class PAGETABLE
{
private:


protected:
    std::vector<unsigned int> numberOfBits; //number of bits per level
    LEVEL* RootNodePtr; // Contains the address of LEVEL 0
    std::vector<unsigned int> shiftArray; //bit shift per level
    std::vector<int> entryCount; // EntryCount[i]: # of possible pages for level i 2^8 for example

public:
    int levelCount; //number of levels in the system
    PAGETABLE(int levCount, std::vector<unsigned int> numOfBits);
    //MAP* PageLookup(unsigned int LogicalAddress);
    void PageInsert(unsigned int LogicalAddress, unsigned int Frame);
    unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);
    void LevelMaskCalc(std::vector<unsigned int> bitsPerLev);
    void ShiftAryCalc(std::vector<unsigned int> bitsPerLev);
    std::vector<unsigned int> GetBitMask();
    std::vector<unsigned int> bitMaskArray; //bit masks per level
};

