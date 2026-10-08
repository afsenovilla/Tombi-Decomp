// FUNC 800450e8 452 MAIN0
// MATCHING 800450e8 452
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern unsigned short DAT_1f80019e;
extern int FUN_800425c4(TObj *, TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8001e4f0(int);

static __inline__ short hit(TObj *a, TObj *b)
{
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return -1;
    if ((unsigned short)((U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2)) + (U16(b, 0x6c) + U16(a, 0x6c))) > S16(b, 0x6e) + S16(a, 0x6e))
        return -1;
    if ((unsigned short)((U16(a, 0x16) - U16(b, 0x16)) + (U16(a, 0x70) + U16(b, 0x70))) > S16(a, 0x72) + S16(b, 0x72))
        return -1;
    return 1;
}

void FUN_800450e8(TObj *a, TObj *b)
{
    int r;

    if (hit(a, b) >= 0) {
        r = 1;
        b->b68 = 1;
        b->b9e = 0;
        switch (FUN_800425c4(a, b)) {
        case 1:
        case 7:
            r = 1;
            break;
        case 4:
        case 5:
        case 10:
            r = 1;
            a->b6a = 1;
            a->active = 2;
            a->wa8 = 0x4ff;
            break;
        case 0:
        case 3:
        case 9:
            r = 2;
            break;
        case 13:
            b->b68 = 0;
            r = -1;
            break;
        case 6:
        case 11:
        case 12:
            r = 2;
            a->b6a = 1;
            a->active = 2;
            a->wa8 = 0x4ff;
            break;
        case 2:
        case 8:
            r = 0;
            b->b9e = 1;
            b->b9f = 0;
            break;
        }
        if (r >= 0) {
            FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
            FUN_8001e4f0((b->category & 0x7f) == 4 ? 6 : 7);
        }
        DAT_1f80019e = 0;
    }
}
