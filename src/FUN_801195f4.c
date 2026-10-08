// FUNC 801195f4 624 X000
// MATCHING 801195f4 624
#include "TOBJ.H"
#define H(o, n) (*(short *)((char *)(o) + (n)))
#define I(o, n) (*(int *)((char *)(o) + (n)))
extern int DAT_1f8002d4;
extern unsigned char DAT_8009c930;
extern unsigned char DAT_800a6047;
extern void FUN_8001fe6c(TObj *);
extern void FUN_80119988(TObj *);
extern void FUN_80119864(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_80026e0c(int, int);
extern void FUN_8001eaa4(int);
extern void FUN_800187e4(TObj *);

typedef struct P { char p[0xcc]; short cc; } P;
void FUN_801195f4(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->w1e = 9;
        o->b0a = 1;
        H(o, 0x2e) = 1;
        o->d3c = DAT_1f8002d4;
        I(o, 0x5c) = 0x1000;
        I(o, 0x60) = 0x1000;
        I(o, 0x64) = 0x1000;
        o->timer = 0;
        o->wb4 = 0;
        o->wb6 = 0;
        o->wb8 = 0;
        ((P *)o)->cc = 0x78;
        switch (o->subtype) {
        case 0:
        case 1:
            o->b0d = 0x80;
            o->step = 10;
            break;
        case 2:
        case 3:
            H(o, 0xc0) = 0;
            H(o, 0xc2) = 0;
            H(o, 0xc4) = 0;
            o->b0d = 0x80;
            o->step = 7;
            H(o, 0xc6) = H(o, 0xc0);
            H(o, 0xc8) = H(o, 0xc2);
            H(o, 0xca) = H(o, 0xc4);
            break;
        case 4:
            o->b0d = 0;
            break;
        case 5:
            break;
        case 6:
            DAT_8009c930 = 2;
        case 7:
            H(o, 0xc0) = 0;
            H(o, 0xc2) = 0;
            H(o, 0xc4) = 0;
            o->b0d = 0x80;
            o->step = 5;
            H(o, 0xc6) = H(o, 0xc0);
            H(o, 0xc8) = H(o, 0xc2);
            H(o, 0xca) = H(o, 0xc4);
            break;
        }
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->subtype != 4) {
            FUN_80119988(o);
            if (o->subtype == 1)
                o->b0f = DAT_800a6047 + 10;
            else
                o->b0f = DAT_800a6047 - 2;
        } else {
            FUN_80119864(o);
            o->b0f = DAT_800a6047;
        }
        FUN_8001fec0(o);
        if (((TObj *)o->d90)->step != 5)
            FUN_800202b4(o);
        if (DAT_8009c930 != 0)
            break;
        o->b04++;
        break;
    case 2:
        if (o->subtype == 0 || o->subtype == 6) {
            FUN_80026e0c(4, 1);
            FUN_8001eaa4(*(unsigned short *)((char *)o + 0xce));
        }
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
