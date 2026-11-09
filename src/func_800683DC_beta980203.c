// VERSION BETA_980203 beta980203 800683DC 56
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/_padGetActSize_scus94236.c (_padGetActSize, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
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
