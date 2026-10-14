// FUNC 8011d8f8 80 X014
// MATCHING 8011d8f8 80
#include "spad.h"

int func_8011D8F8(unsigned short *p)
{
    if ((unsigned short)(p[0] - (*(unsigned short*)((char*)&SPAD.iec + 2)) + 0xa0) < 0x141)
        return (unsigned short)(p[1] - (*(unsigned short*)((char*)&SPAD.if0 + 2)) + 0x6e) < 0xdd;
    return 0;
}
