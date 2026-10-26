// FUNC 800f0e84 1000 I0NFO2
// MATCHING 800f0e84 1000
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short a, b, c; } Lim;
typedef struct { int **anims; int p4; int p8; } AnimSet;
extern void AnimLoadDuration(TObj *o, int n);
extern void ObjListPush_1F800220(TObj *o);
extern unsigned char D_8009CF9F[];
extern unsigned char D_8009CFA1;
extern unsigned char D_8009CFA2[];
extern TObj PLAYER;
extern unsigned char D_800A60A3;
extern Lim D_800FEDF4[];
extern AnimSet D_800FEE0C[];

void topdownA17SceneryUpdate(TObj *o)
{
    short k;
    unsigned char c, d;

    switch ((unsigned short)S16(o, 0xB8)) {
    case 0xD:
    case 0xE:
        k = S16(o, 0xB8) - 0xD;
        switch (U8(o, 0x5)) {
        case 0:
            c = D_8009CF9F[k] & 3;
            d = D_8009CFA2[k];
            if (c != d) {
                switch (c) {
                case 0:
                    U8(o, 0x5) = 3;
                    break;
                case 1:
                    if (d == 2) {
                        U8(o, 0x5) = 4;
                    } else {
                        U8(o, 0x5) = 6;
                    }
                    break;
                case 2:
                    U8(o, 0x5) = 5;
                    break;
                }
            }
            break;
        case 1:
            D_8009CF9F[k] = (D_8009CF9F[k] & 0x80) + 1;
            U8(o, 0x5) = 0;
            break;
        case 2:
            D_8009CF9F[k] = 0x82;
            U8(o, 0x5) = 0;
            break;
        case 3:
            if (D_8009CF9F[k] & 0x40) {
                PLAYER.a.p.whole++;
            }
            o->a.p.whole++;
            if (o->a.p.whole == D_800FEDF4[k].b) {
                U8(o, 0x5) = 1;
            }
            break;
        case 4:
            if (D_8009CF9F[k] & 0x40) {
                PLAYER.a.p.whole++;
            }
            o->a.p.whole++;
            if (o->a.p.whole == D_800FEDF4[k].c) {
                U8(o, 0x5) = 2;
            }
            break;
        case 5:
            if (D_8009CF9F[k] & 0x40) {
                PLAYER.a.p.whole--;
            }
            o->a.p.whole--;
            if (o->a.p.whole == D_800FEDF4[k].b) {
                U8(o, 0x5) = 1;
            }
            break;
        case 6:
            if (D_8009CF9F[k] & 0x40) {
                PLAYER.a.p.whole--;
            }
            o->a.p.whole--;
            if (o->a.p.whole == D_800FEDF4[k].a) {
                U8(o, 0x5) = 7;
            }
            break;
        case 7:
            D_8009CF9F[k] = 0;
            U8(o, 0x5) = 0;
            break;
        }
        break;
    case 0xF:
        { register int dd asm("a1"); dd = D_8009CFA1; if (U16(o, 0xB4) != (unsigned short)(dd + 0xD)) {
            { register int m asm("v1"); m = dd + 0xD; U16(o, 0xB4) = m; }
            PTR(o, 0x24) = (void *)D_800FEE0C[U8(o, 0x3)].anims[U16(o, 0xB4)];
            AnimLoadDuration(o, dd);
        }
        } break;
    }
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == U8(o, 0xB6)) {
            U8(o, 0x6B) = 0x80;
        } else {
            U8(o, 0x6B) = 0;
        }
    } else if (U16(o, 0xB6) != 0) {
        U8(o, 0x6B) = 0;
    } else {
        U8(o, 0x6B) = 0x80;
    }
    U8(o, 0x1) = 1;
    ObjListPush_1F800220(o);
}
