// VERSION SLPS_01144 x16 80116128 376
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;
typedef struct { short s0; unsigned short s2; } U2;
extern unsigned short D_8009E52A;
extern unsigned char D_8009E4E2;
extern S2 *D_800A7BBC;
extern char D_800A7B78[];
extern void PlayerSetAnimIfChanged(TObj *, int);

void func_80116128_slps01144(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_8009E52A != 0) {
            if (D_800A7BBC->s2 < 0x50) {
                PlayerSetAnimIfChanged((TObj *)D_800A7B78, 0x2c);
                PLAYER.obj.b04 = 5;
                PLAYER.obj.step = 0x65;
                PLAYER.obj.state = 0;
                D_8009E4E2 = 1;
                o->state = 1;
            }
        } else if (D_800A7BBC->s2 < 0x64) {
            PlayerSetAnimIfChanged((TObj *)D_800A7B78, 0x2b);
            PLAYER.obj.b04 = 5;
            PLAYER.obj.step = 0x65;
            D_8009E4E2 = 1;
            PLAYER.obj.state = 0;
            o->state = 2;
        }
        break;
    case 1:
        ((U2 *)D_800A7BBC)->s2--;
        break;
    case 2:
        ((U2 *)D_800A7BBC)->s2++;
        break;
    }
}
#endif
