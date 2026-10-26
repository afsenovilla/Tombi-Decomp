// FUNC 8010a168 944 X009
// MATCHING 8010a168 944
#include "gv6.h"
#include "TOBJ.H"
typedef struct { short v[9]; } T9;
extern T9 D_800E989C;
extern unsigned char *PLAYER_EXT;

void playerSpinAimDir8A(TObj *o)
{
    T9 t = D_800E989C;
    volatile unsigned short *k;
    PLAYER_EXT[0xb] = 8;
    if (o->animFrame & 1) {
        k = &GV6.s70;
        if (*k & 0x10) {
            PLAYER_EXT[0xb] = 0;
            if (*k & 0x80) PLAYER_EXT[0xb] = 7;
            if (*k & 0x20) PLAYER_EXT[0xb] = 1;
        } else if (*k & 0x80) {
            PLAYER_EXT[0xb] = 6;
            if (*k & 0x10) PLAYER_EXT[0xb] = 7;
            if (*k & 0x40) PLAYER_EXT[0xb] = 5;
        } else if (*k & 0x40) {
            PLAYER_EXT[0xb] = 4;
            if (*k & 0x80) PLAYER_EXT[0xb] = 5;
            if (*k & 0x20) PLAYER_EXT[0xb] = 3;
        } else if (*k & 0x20) {
            PLAYER_EXT[0xb] = 2;
            if (*k & 0x10) PLAYER_EXT[0xb] = 1;
            if (*k & 0x40) PLAYER_EXT[0xb] = 3;
        }
    } else {
        k = &GV6.s70;
        if (*k & 0x10) {
            PLAYER_EXT[0xb] = 0;
            if (*k & 0x80) PLAYER_EXT[0xb] = 1;
            if (*k & 0x20) PLAYER_EXT[0xb] = 7;
        } else if (*k & 0x80) {
            PLAYER_EXT[0xb] = 2;
            if (*k & 0x10) PLAYER_EXT[0xb] = 1;
            if (*k & 0x40) PLAYER_EXT[0xb] = 3;
        } else if (*k & 0x40) {
            PLAYER_EXT[0xb] = 4;
            if (*k & 0x80) PLAYER_EXT[0xb] = 3;
            if (*k & 0x20) PLAYER_EXT[0xb] = 5;
        } else if (*k & 0x20) {
            PLAYER_EXT[0xb] = 6;
            if (*k & 0x10) PLAYER_EXT[0xb] = 7;
            if (*k & 0x40) PLAYER_EXT[0xb] = 5;
        }
    }
    o->d88 = t.v[PLAYER_EXT[0xb]];
}
