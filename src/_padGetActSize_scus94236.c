// VERSION SCUS_94236 scus94236 80069C98 56
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (psyq/libpad/padcmd.c, _padGetActSize); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
int _padGetActSize(padPort *arg0) {
    int first;
    int second;
    int responseBytes;
    int base;
    int raw_first;
    int raw_second;

    raw_first = arg0->modeCount;
    raw_second = arg0->actuatorCount;
    base = arg0->combinationStorageBytes;

    first = raw_first + 1;
    first >>= 1;
    first <<= 2;

    responseBytes = (raw_second << 2) + raw_second;
    second = (responseBytes + 3) & 0xFFC;
    second += 4;

    first += second;
    return first + base;
}
#endif
