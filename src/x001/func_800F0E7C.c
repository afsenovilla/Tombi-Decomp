// FUNC 800f0e7c 1164 X001
// MATCHING 800f0e7c 1164
#include "TOBJ.H"
typedef struct {
    unsigned char p0[0x1e];
    unsigned char b1e;
    unsigned char b1f;
    unsigned char p20[0x2c - 0x20];
    unsigned short w2c;
    unsigned short w2e;
} Q800F0E7C;

extern Q800F0E7C *D_8009C330;
extern unsigned short D_8009C962;
extern unsigned short D_8009C960;
extern unsigned char D_8009CDB2;
extern unsigned char D_8009CDB5;
extern short FUN_800f0b5c();
extern short FUN_800f0c28(TObj *);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimJump(TObj *, int);

#define B(o, off) (((unsigned char *)(o))[off])

#define SETANIM(n) \
    D_8009C330->w2c = n; \
    ObjSetAnimFromTable(o); \
    AnimJump(o, 0); \
    D_8009C330->w2e = D_8009C330->w2c;

#define SETANIM_IF(n) \
    D_8009C330->w2c = n; \
    if (D_8009C330->w2e != n) { \
        D_8009C330->w2c = n; \
        ObjSetAnimFromTable(o); \
        AnimJump(o, 0); \
        D_8009C330->w2e = D_8009C330->w2c; \
    }

short func_800F0E7C(TObj *o)
{
    short r = 0;
    unsigned short f = o->animFrame;

    if (f & 2) {
        if (f & 8) {
            D_8009C330->b1f = 0;
            switch (B(o, 0xa0) & 0xf) {
            case 1:
                B(o, 0xa2) = 1;
                if (FUN_800f0b5c()) {
                    SETANIM(0x49);
                    if (B(o, 0xa8)) {
                        r = FUN_800f0c28(o);
                    } else {
                        r = 3;
                        D_8009C330->b1e++;
                    }
                }
                break;
            case 2:
                if (D_8009C962 == 1 && (D_8009C960 == 2 || D_8009C960 == 0x13)) {
                    if (D_8009CDB2 != 0xff || D_8009CDB5 != 0xff) {
                        return r;
                    }
                }
                SETANIM(0x14);
                r = 1;
                B(o, 0xa2) = 1;
                break;
            case 3:
                SETANIM_IF(0x24);
                r = 5;
                break;
            case 4:
                SETANIM_IF(0x24);
                B(o, 0xa2) = 1;
                r = 4;
                break;
            case 5:
                SETANIM_IF(0x24);
                B(o, 0xa2) = 1;
                r = 0x75;
                break;
            }
        } else if (f & 4) {
            D_8009C330->b1e = 0;
            switch (B(o, 0xa0) >> 4) {
            case 1:
                SETANIM(0x18);
                r = 2;
                B(o, 0xa3) = 1;
                break;
            case 2:
                SETANIM(0x48);
                r = 7;
                D_8009C330->b1f++;
                break;
            case 3:
                SETANIM_IF(0x24);
                r = 6;
                break;
            case 4:
                SETANIM(0x18);
                B(o, 0xa3) = 1;
                D_8009C330->b1f++;
                r = 8;
                break;
            }
        } else {
            D_8009C330->b1e = 0;
            D_8009C330->b1f = 0;
        }
    }
    return r;
}
