#include <iostream>

struct  LEVEL
{
};

struct MAP
{

};

struct PAGETABLE 
{
    int levelCount;
    unsigned int blah[10];
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