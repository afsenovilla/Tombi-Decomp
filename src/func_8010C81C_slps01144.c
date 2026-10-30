// near-twin of playerSwimDirFromPadB (PAL)
// VERSION SLPS_01144 x00 8010C81C 520
#if defined(VERSION_SLPS_01144)
#define DAT_8009ce3d D_8009E9E5
#define PAD_PRESSED D_8009F200
#include "TOBJ.H"
extern unsigned char DAT_8009ce3d;
extern unsigned short PAD_PRESSED;

void func_8010C81C_slps01144(TObj *o)
{
    char pad;
    volatile unsigned short *p;
    if (DAT_8009ce3d != 0xff) {
        p = &PAD_PRESSED;
        if (*p & 0x2000) {
            o->w76 = 0;
            o->wb6 = 0;
        }
        if (*p & 0x8000) {
            o->w76 = 4;
            o->wb6 = 0x80;
        }
    } else if (p = &PAD_PRESSED, !(*p & 0xe000)) {
        if (o->timer != 0) {
            if (--o->timer <= 0) {
                if ((unsigned)((unsigned short)o->wb6 - 0x40) < 0x80)
                    o->w76 = 4;
                else
                    o->w76 = 0;
                o->w76 |= 8;
            }
        }
    } else {
        o->timer = 10;
        if (*p & 0x2000) {
            if (*p & 0x4000) {
                if ((unsigned)((unsigned short)o->wb6 - 0x50) <= 0x20)
                    o->wb6 = 0xe0;
                o->w76 = 7;
            } else {
                if (o->w76 == 4 && o->wb6 == 0x80)
                    o->wb6 = 0;
                o->w76 = 0;
            }
        } else if (*p & 0x8000) {
            if (*p & 0x4000) {
                if ((unsigned)((unsigned short)o->wb6 - 0x10) <= 0x20)
                    o->wb6 = 0xa0;
                o->w76 = 5;
            } else {
                if (o->w76 == 0 && o->wb6 == 0)
                    o->wb6 = 0x80;
                o->w76 = 4;
            }
        } else if (*p & 0x4000) {
            if ((unsigned)((unsigned short)o->wb6 - 0x30) <= 0x20)
                o->wb6 = 0xc0;
            o->w76 = 6;
        }
    }
}
#endif
