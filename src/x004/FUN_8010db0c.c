// FUNC 8010db0c 568 X004
// MATCHING 8010db0c 568
#include "TOBJ.H"
typedef struct {
    char pad0[8];
    unsigned char b8;
    char pad9[0x17];
    short w20;
    char pad22[0xa];
    short w2c;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
extern G330 *DAT_8009c330;
extern short DAT_8009c944[2];
extern int DAT_8009f0ec;
extern void FUN_8001e560(int, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_800ef05c(int, int, int, int);
extern void FUN_800ee680(TObj *);
extern void FUN_800eec40(TObj *);
extern void FUN_8003facc(TObj *);
extern void FUN_800eea7c(TObj *, int, int);
extern int FUN_8004bbc0(TObj *, int);
extern void FUN_800efc8c(TObj *, int);
extern void FUN_800eef0c(void);

void FUN_8010db0c(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        OB(o, 0xca) = 0;
        OB(o, 0xac) = 0;
        o->velY = -0x780;
        o->d8c = 0;
        o->wb0 = 0;
        o->velX = 0;
        DAT_8009c330->w20 = 0xe;
        FUN_8001e560(2, 4);
        FUN_8001e5f4(0xe, 0x7f);
        o->state++;
        break;
    case 1:
        o->h->raw += DAT_8009c944[0] << 8;
        o->y.raw += DAT_8009c944[1] << 8;
        FUN_8001fec0(o);
        if (o->ba7)
            FUN_800ef05c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_800ee680(o);
        if (o->velY >= -0x383)
            FUN_800eec40(o);
        FUN_8003facc(o);
        if (o->velY > 0) {
            DAT_8009c330->w2c = 4;
            FUN_800eea7c(o, 4, 1);
            OB(o, 0xac) = 1;
            DAT_8009c330->b8 = 1;
            o->velY = 0;
            o->velV = 0;
            o->b9c = 2;
            o->timer = 10;
            o->d84 = 0;
            o->d88 = (o->animFrame & 1) ? 0xf0 : 0x10;
            o->d8c = (o->animFrame & 1) ? 0x40 : 0xc0;
            o->step = 2;
            o->state = 3;
        }
        break;
    }
    DAT_8009f0ec = FUN_8004bbc0(o, 0);
    if (o->b9e) {
        DAT_8009c330->b8 = 0;
        DAT_8009c330->w20 = 0;
        OB(o, 0xac) = 0;
        o->b9c = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        FUN_800efc8c(o, DAT_8009f0ec == 1);
    }
    FUN_800eef0c();
}
