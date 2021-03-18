#pragma once

struct MAP;
struct PAGETABLE;
struct LEVEL;
unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);
MAP* PageLookup(PAGETABLE* PageTable, unsigned int LogicalAddress);
void PageInsert();