// FUNC 8005d274 84 MAIN0
// MATCHING 8005d274 84
// Portado de psx_tomba (psyq/libcard/init.c, InitCARD); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

void InitCARD(long val)
{
    ChangeClearPAD(0);
    EnterCriticalSection();
    InitCARD2(val);
    _patch_card();
    _patch_card2();
    ExitCriticalSection();
    return;
}

void StartCARD(void);

void StopCARD(void);
