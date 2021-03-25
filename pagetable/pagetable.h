#pragma once
extern unsigned int frame;
extern const int SYSTEMSIZE;

class MAP
{
public:
    int index;
    bool validFrame;
    MAP();
    MAP(int index, bool validFrame);
};

class PAGETABLE;
class LEVEL;

class LEVEL
{
public:
    LEVEL(int depth, PAGETABLE& PageTable);
    int DepthOfLevel; // Which level is this?
    PAGETABLE* PageTablePtr;   // Point to the PageTable that contains the root level node
    std::vector<LEVEL*>* NextLevelPtr;   // Array of pages in this level, each element points to a level node in the next level
    std::vector<MAP>* MapPtr;
};

class PAGETABLE
{
public:
    int levelCount; //number of levels in the system
    //LEVEL rootLevel;
    LEVEL* RootNodePtr; // Contains the address of LEVEL 0
    PAGETABLE(int levCount, std::vector<unsigned int> numOfBits);
    MAP* PageLookup(unsigned int LogicalAddress);
    void PageInsert(unsigned int LogicalAddress, unsigned int Frame);
    void PageInsert(LEVEL* levelPtr, unsigned int LogicalAddress, unsigned int Frame);
    unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);
    void LevelMaskCalc(std::vector<unsigned int> bitsPerLev);
    void ShiftAryCalc(std::vector<unsigned int> bitsPerLev);
    std::vector<unsigned int> numberOfBits; //number of bits per level
    std::vector<unsigned int> shiftArray; //bit shift per level
    std::vector<int> entryCount; // EntryCount[i]: # of possible pages for level i 2^8 for example
    std::vector<unsigned int> GetBitMask();
    uint32_t GetMaskTot();
    std::vector<unsigned int> bitMaskArray; //bit masks per level
    std::vector<unsigned int> GetNumberOfBits();
    std::vector<unsigned int> GetShiftArray();
    uint32_t FramePlusOffSet(uint32_t address, uint32_t frame, uint32_t mask, unsigned int physmap);
    unsigned int ByteCalc();
};