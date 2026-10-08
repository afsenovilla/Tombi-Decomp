// FUNC 80109494 856 X019
// MATCHING 80109494 856
#include "TOBJ.H"
typedef struct {
    char pad0[0x20];
    short w20;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
#define WF6(o) (*(short *)((char *)(o) + 0xf6))
extern G330 *DAT_8009c330;
extern unsigned short DAT_8009c960;
extern unsigned char DAT_8009c93a[];
extern unsigned char DAT_8009ce61;
extern unsigned char DAT_8009d119;
extern unsigned char DAT_801152e8[];
extern char DAT_80010e34[];
extern int FUN_8001fddc(int, int);
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001e76c(int, int, int, int);
extern void FUN_8001eb64(void);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8005a8a8(int, int, int);

void FUN_80109494(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        OB(o, 0xa3) = 2;
        o->wb6 = 0;
        o->animFrame &= 1;
        o->velX = FUN_8001fddc(0, o->wb2);
        o->velY = -0x570;
        DAT_8009c330->w20 = 5;
        FUN_800eeb5c(o, 0x19);
        if (DAT_8009c960 == 0) {
            FUN_8001e76c(0xd, 1, -8, 0x96);
            FUN_8001eb64();
        }
        o->state = 1;
    case 1:
        FUN_8010f0f4(o);
        if (DAT_8009c960 == 0)
            o->d->raw -= 0x80000;
        else
            o->d->raw -= 0x50000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 2;
        }
        if (FUN_8003facc(o)) {
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 2;
        }
        break;
    case 2:
        o->wb0 = 0;
        o->wb6 = 0;
        FUN_8010f0f4(o);
        o->d->raw -= 0x28000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->b69 == 1 || FUN_8003fd78(o, 0, 0)) {
            FUN_8001e5f4(0x1c, 0x7f);
            OB(o, 0xa3) = 1;
            o->anim = DAT_80010e34;
            FUN_8001fe94(o, 2);
            o->d->p.whole = WF6(o);
            o->d8c = DAT_801152e8[o->wb0];
            OB(o, 0xac) = 0;
            o->state = 3;
            OB(o, 0xa3) = 0;
        }
        break;
    case 3:
        o->velY = 0x780;
        FUN_8001fd94(o);
        FUN_8003fd78(o, 0, 0);
        if (FUN_8001fec0(o)) {
            DAT_8009c93a[0] = 1;
            *(signed char *)&o->b0f = -8;
            OB(o, 0xa5) = 0;
            OB(o, 0xa3) = 0;
            o->b9c = 0;
            OB(o, 0xac) = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            o->d8c = DAT_801152e8[o->wb0];
            DAT_8009c330->w20 = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->active = 1;
            if (DAT_8009ce61 == 0 && DAT_8009d119 != 0)
                FUN_8005a8a8(0xbd, 0, 1);
        }
        break;
    }
}
