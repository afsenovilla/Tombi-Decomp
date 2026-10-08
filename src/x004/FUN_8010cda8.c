// FUNC 8010cda8 520 X004
// MATCHING 8010cda8 520
#include "TOBJ.H"
extern unsigned char DAT_8009ce3d;
extern unsigned short DAT_8009d670;

void FUN_8010cda8(TObj *o)
{
    char pad;
    volatile unsigned short *p;
    if (DAT_8009ce3d != 0xff) {
        p = &DAT_8009d670;
        if (*p & 0x20) {
            o->w76 = 0;
            o->wb6 = 0;
        }
        if (*p & 0x80) {
            o->w76 = 4;
            o->wb6 = 0x80;
        }
    } else if (p = &DAT_8009d670, !(*p & 0xe0)) {
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
        if (*p & 0x20) {
            if (*p & 0x40) {
                if ((unsigned)((unsigned short)o->wb6 - 0x50) <= 0x20)
                    o->wb6 = 0xe0;
                o->w76 = 7;
            } else {
                if (o->w76 == 4 && o->wb6 == 0x80)
                    o->wb6 = 0;
                o->w76 = 0;
            }
        } else if (*p & 0x80) {
            if (*p & 0x40) {
                if ((unsigned)((unsigned short)o->wb6 - 0x10) <= 0x20)
                    o->wb6 = 0xa0;
                o->w76 = 5;
            } else {
                if (o->w76 == 0 && o->wb6 == 0)
                    o->wb6 = 0x80;
                o->w76 = 4;
            }
        } else if (*p & 0x40) {
            if ((unsigned)((unsigned short)o->wb6 - 0x30) <= 0x20)
                o->wb6 = 0xc0;
            o->w76 = 6;
        }
    }
}
