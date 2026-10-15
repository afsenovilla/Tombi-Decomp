// FUNC 801274b0 76 X004
// MATCHING 801274b0 76


#include "globals.h"
void setScriptGlobalFlagBit(int n)
{
    int q = n / 8;
    int i = q + 0x58;
    SCRIPT_GLOBALS[i] |= 1 << (n - q * 8);
}
