// FUNC 8011f1d0 184 X000
// wip: solo difiere el addu final (orden operandos) y regs
#include "raw7.h"
extern char *G;
extern unsigned char DAT_8009cdac;

extern void FUN_80018cf0(void);
extern void FUN_80018838(void);
void FUN_8011f1d0(char *o)
{
    switch (U8(o, 4)) {
    case 0:
        U8(o, 4)++;
        U8(o, 0) = 2;
        if (DAT_8009cdac == 0xff)
            { char * volatile *pg = &G; S32(o, 0xa0) = (int)*pg + *(int *)(*pg + 0x30); }
        break;
    case 1:
        U8(o, 1) = 1;
        FUN_80018cf0();
        break;
    case 2:
    case 3:
        FUN_80018838();
        break;
    }
}
