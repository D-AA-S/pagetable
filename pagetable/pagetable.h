#pragma once

struct MAP;
struct PAGETABLE {
    int levelCount;
    unsigned int* numberOfBits;
    unsigned int* bitMaskAry;
    unsigned int* shiftAry;
    int* entryCount;
    PAGETABLE(int levelCount, unsigned int* numberOfBits);
};
struct LEVEL;
unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);