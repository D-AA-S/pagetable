#include <iostream>
#include <vector>
#include <cmath>
#include "pagetable.h"
#include <bitset>

const int SYSTEMSIZE = 32;  // The bit size of the machine set to a constant of 32
unsigned int frame = 0; // Global variable to hold the working frames

/**
 * MAP()
 * 
 * This is the default MAP constructor. It initializes the member variables to a default amount.
 */
MAP::MAP()
{
    index = -1;
    validFrame = false;
}

/**
 * MAP(int index, bool validFrame)
 *
 * This is the MAP constructor. This constructor is to be used when a page needs to be inserted.
 * It sets the index to the current frame available and sets the validFrame boolean to true;
 * 
 * @param index: the currents frame to be assigned
 * @param validFrame: information about the validity of the MAP frame.
 */
MAP::MAP(int index, bool validFrame)
{
    this->index = index;
    this->validFrame = validFrame;
}

/**
 * LEVEL(int depth, PAGETABLE& PageTable)
 *
 * This is the LEVEL constructor. This constructor is to be used when a new LEVEL needs to be
 * instantiated. It sets DepthofLevel to the next lowest depth and initializes the PageTablePtr
 * with a reference to the PageTable.
 * 
 * @param depth: the current depth of the tree
 * @param &PageTable: a reference to the working PageTable
 */
LEVEL::LEVEL(int depth, PAGETABLE& PageTable)
{
    DepthOfLevel = depth;
    PageTablePtr = &PageTable;
};

/**
 * PageInsert(unsigned int LogicalAddress, unsigned int Frame)
 *
 * This function calls the auxiliary PageInsert function to begin the recursive page walk.
 * 
 * @param LogicalAddress: The logical address to be translated to a page and inserted into the PageTable
 * @param Frame: The value of the currently available frame.
 */
void PAGETABLE::PageInsert(unsigned int LogicalAddress, unsigned int Frame) // Used to add new entries to the page table when we have discovered that a page has not yet been allocated(PageLookup returns NULL).
{
    PageInsert(RootNodePtr, LogicalAddress, Frame);
}

/**
 * PageInsert(LEVEL* levelPtr, unsigned int LogicalAddress, unsigned int Frame)
 *
 * This function is the auxiliary PageInsert Function. This function calls itself when it needs to traverse
 * further into the PageTable. It inserts the page at the correct index in the MAP when it reaches the bottom.
 * If either the NextLevelPtr or the MapPtr points to NULL, it instantiates a new vector of LEVEL pointers and
 * MAPs as needed.
 * 
 * @param levelPtr: This is a pointer to the current LEVEL. Dereferening the pointer gives access to the LEVEL members.
 * @param LogicalAddress: The logical address to be translated to a page and inserted into the PageTable
 * @param Frame: The value of the currently available frame.
 */
void PAGETABLE::PageInsert(LEVEL* levelPtr, unsigned int LogicalAddress, unsigned int Frame)
{
    unsigned int currentDepth = levelPtr->DepthOfLevel;
    unsigned int bitMask = levelPtr->PageTablePtr->bitMaskArray[currentDepth];  //Get the bitmask for the current level
    unsigned int entryCount = levelPtr->PageTablePtr->entryCount[currentDepth];
    unsigned int shift = levelPtr->PageTablePtr->shiftArray[currentDepth];
    unsigned int pageIndex = LogicalToPage(LogicalAddress, bitMask, shift);

    if (currentDepth == levelCount - 1)   // If the current depth is equal to the levelCount - 1, then we are at a leaf node
    {
        if (levelPtr->MapPtr == NULL)   //There are no maps for this address yet; create a new map array
            levelPtr->MapPtr = new std::vector<MAP>(entryCount);
        if (levelPtr->MapPtr->at(pageIndex).validFrame == true)    // there already exists a mapping here. Do not need to replace
            return;
        MAP* newMap = new MAP(Frame, true);
        levelPtr->MapPtr->at(pageIndex) = *newMap;
        frame++;
    }
    else
    {
        if (levelPtr->NextLevelPtr == NULL) // If the NextLevelPtr points to nothing, create a new vector of LEVEL ptrs of size entrycount
            levelPtr->NextLevelPtr = new std::vector<LEVEL*>(entryCount);
        if (levelPtr->NextLevelPtr->at(pageIndex) == NULL)
            levelPtr->NextLevelPtr->at(pageIndex) = new LEVEL(currentDepth + 1, *this);
        LEVEL* newLevel = levelPtr->NextLevelPtr->at(pageIndex);
        PageInsert(newLevel, LogicalAddress, Frame);
    }
}

/**
 * PAGETABLE(int levCount, std::vector<unsigned int> numOfBits)
 *
 * This is the PAGETABLE constructor. This is expected to only be called once because there should
 * only be one instance of the PAGETABLE. The PageTable is a tree data structure consisting of LEVEL
 * nodes. The LEVEL nodes contain pointers that point to a vector of LEVEL pointers which then contain
 * the next level. At the bottom of the PageTable, the LEVEL contains a MAP pointer which points to a
 * vector of MAPs. Each MAP contains the frame the VPN was mapped to as well as information about the
 * validity of the MAP.
 * 
 * @param levCount: The number of levels in the PageTable
 * @param numOfBits: The number of bits at each level in the PageTable
 */
PAGETABLE::PAGETABLE(int levCount, std::vector<unsigned int> numOfBits)
{
    levelCount = levCount; //number of levels in the system; For example: 3 levels
    numberOfBits = numOfBits; //number of bits per level; For example: [8, 8, 8]
    LevelMaskCalc(numberOfBits);    // Assigns the bitMaskArray with the appropriate bitmasks per level; For example: [FF000000, 00FF0000, 0000FF00]
    ShiftAryCalc(numberOfBits);     // Assigns the shiftArray with the appropriate bits to shift per level; For example: [24, 16, 8]

    for (int i = 0; i < levelCount; i++)    // Assigns the entryCount vector with the number of possible pages per level
        entryCount.push_back(pow(2, numberOfBits[i]));

    // Setup Level 0
    RootNodePtr = new LEVEL(0, *this); // Assign the address of the newly created LEVEL to the RootNodePtr. This points to Level 0
};

/**
 * PageLookup(unsigned int LogicalAddress)
 *
 * PageLookup is intended to be called before the PageInsert function. PageLookup determines whether there already exists a valid
 * MAP for the LogicalAddress in the PageTable. If there is a valid MAP, then return a pointer to that MAP. If there doesn't exists
 * a valid MAP, then it returns NULL, which prompts the PageInsert function.
 *
 * @param LogicalAddress: The LogicalAddress to be searched in the PageTable
 */
MAP* PAGETABLE::PageLookup(unsigned int LogicalAddress)
{
    MAP* map = new MAP();
    unsigned int currentDepth;
    LEVEL current = *RootNodePtr;

    for (int i = 0; i < levelCount; i++)
    {
        currentDepth = current.DepthOfLevel;
        unsigned int page = LogicalToPage(LogicalAddress, bitMaskArray[i], shiftArray[i]);
        if (currentDepth == levelCount - 1) // We are at the maps. Check to see if the map exisits. If it doesn't, return null. If it does, return the map!
        {
            if (current.MapPtr == NULL)
                return NULL;
            else if (current.MapPtr->at(page).validFrame == false)
                return NULL;
            else
            {
                map = &(current.MapPtr->at(page));
                return map;
            }
        }
        else
        {
            if (current.NextLevelPtr == NULL)
                return NULL;
            else if (current.NextLevelPtr->at(page) == NULL)
                return NULL;
            else
                current = *(current.NextLevelPtr->at(page));
        }
    }
}

/**
 * LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift)
 *
 * LogicalToPage performs the translation from the Logical address to a page in the PageTable.
 * The page is used to find the correct index either in the vector of LEVEL pointers or the
 * vector of MAPs.
 *
 * @param LogicalAddress: The LogicalAddress that will be translated to a page
 * @param Mask: a bit mask that will be used to grab the relevant address for the current level
 * @param Shift: The number of bits that need to be shifted to get the correct page
 */
unsigned int PAGETABLE::LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift)
{
    unsigned int page = LogicalAddress & Mask;
    page >>= Shift;
    return page;    // returns an unsigned int containing the pageNumber. Check to see if a LEVEL exists in the array of LEVEL pointers
}

void PAGETABLE::LevelMaskCalc(std::vector<unsigned int> bitsPerLev)
{
    unsigned int tempBitMask = 0b0;
    int shift = 0;
    for (int i = 0; i < bitsPerLev.size(); i++)
    {
        for (int j = bitsPerLev[i]; j > 0; j--) {
            tempBitMask <<= 1;
            tempBitMask |= 0b1;
        }
        tempBitMask <<= SYSTEMSIZE - bitsPerLev[i] - shift;
        shift += bitsPerLev[i];
        bitMaskArray.push_back(tempBitMask);
        tempBitMask = 0;
    }
}

void PAGETABLE::ShiftAryCalc(std::vector<unsigned int> bitsPerLev)
{
    int shift = 0;
    for (int i = 0; i < bitsPerLev.size(); i++)
    {
        shiftArray.push_back(SYSTEMSIZE - bitsPerLev[i] - shift);
        shift += bitsPerLev[i];
    }
}

uint32_t PAGETABLE::GetMaskTot()
{
    uint32_t maskTot = 0;
    for (int i = 0; i < bitMaskArray.size(); i++)
    {
        maskTot |= bitMaskArray[i];
    }
    return maskTot;
}

std::vector<unsigned int> &PAGETABLE::GetBitMask()
{
    return bitMaskArray;
}

std::vector<unsigned int> &PAGETABLE::GetShiftArray() 
{
    return shiftArray;
}

std::vector<unsigned int> &PAGETABLE::GetNumberOfBits()
{
    return numberOfBits;
}

uint32_t PAGETABLE::FramePlusOffSet(uint32_t address, uint32_t frame, uint32_t mask, unsigned int physmap)
{
    address = address & ~mask;
    frame<<=(SYSTEMSIZE-physmap);
    address = frame +address;
    return address; 
}

unsigned int PAGETABLE::ByteCalc(PAGETABLE instance, int pagesize)
{
    unsigned int byteTot = 0;
    for (int i = 0; i < instance.levelCount; i++) 
    {
        byteTot = byteTot + (pow(2, (numberOfBits[i])))*4 + pagesize;
    }
    return byteTot;
}