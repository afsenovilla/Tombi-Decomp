// FUNC 8005d2c8 56 MAIN0
// MATCHING 8005d2c8 56
// Portado de psx_tomba (psyq/libcard/init.c, StartCARD); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

void InitCARD(long val);

void StartCARD(void) {
    EnterCriticalSection();
    StartCARD2();
    ChangeClearPAD(0);
    ExitCriticalSection();
    return;
}

void StopCARD(void);
