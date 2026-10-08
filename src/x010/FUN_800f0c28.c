// FUNC 800f0c28 596 X010
// MATCHING 800f0c28 596
#include "TOBJ.H"
typedef struct {
    unsigned char _p00[0x1e];
    unsigned char b1e;
    unsigned char _p1f[0xf];
    unsigned short w2e;
} PL;
extern void FUN_8001fe6c(TObj *);
extern char DAT_80010de0[];
extern PL *DAT_8009c330;
extern unsigned char DAT_8009d2b1;
extern unsigned short DAT_8009c960;
extern unsigned char DAT_8009ce0b;
extern unsigned char DAT_8009d0da;
extern unsigned char DAT_8009d0d9;
extern unsigned char DAT_8009d2c3;
extern unsigned char DAT_8009cdc8;
extern unsigned char DAT_8009cdc5;

int FUN_800f0c28(TObj *o)
{
    int r = 0;

    switch (*(unsigned char *)&o->wa8) {
    case 1:
        r = 0xff;
        if (DAT_8009d2b1 == 1) {
            r = 10;
            if (DAT_8009c960 == 3) {
                r = 0xc;
                o->w7a = 7;
            }
        }
        break;
    case 2:
        r = 0xff;
        if (DAT_8009d2b1 == 2) r = 9;
        break;
    case 3:
        r = 0xff;
        break;
    case 4:
        if (DAT_8009ce0b == 0) {
            DAT_8009c330->w2e = 0xffff;
            o->anim = DAT_80010de0;
            FUN_8001fe6c(o);
            r = 1;
        } else {
            DAT_8009c330->b1e++;
            r = 3;
        }
        break;
    case 5:
        if (DAT_8009d0da != 0) {
            DAT_8009c330->b1e++;
            r = 3;
        } else r = 0xff;
        break;
    case 6:
        if (DAT_8009d0d9 != 0) {
            DAT_8009c330->b1e++;
            r = 3;
        } else r = 0xff;
        break;
    case 7:
        if (DAT_8009d2c3 & 2) {
            DAT_8009c330->b1e++;
            o->w7a = 8;
            r = 0xb;
        } else r = 0xff;
        break;
    case 8:
        DAT_8009c330->b1e++;
        r = 3;
        break;
    case 9:
        DAT_8009c330->w2e = 0xffff;
        o->anim = DAT_80010de0;
        FUN_8001fe6c(o);
        r = 0xd;
        break;
    case 10:
        if (DAT_8009cdc8 == 0xff) {
            DAT_8009c330->b1e++;
            r = 0xb;
            o->w7a = 9;
        } else r = 0xff;
        break;
    case 11:
        if (DAT_8009d0d9 != 0) {
            DAT_8009c330->b1e++;
            o->w7a = 0x25;
            r = 0xb;
        } else r = 0xff;
        break;
    case 12:
        if (DAT_8009cdc5 == 0xff) {
            DAT_8009c330->b1e++;
            r = 0xb;
            o->w7a = 0x37;
        } else r = 0xff;
        break;
    }
    return r;
}
