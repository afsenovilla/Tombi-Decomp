// FUNC 80044d54 452 MAIN0
// MATCHING 80044d54 452
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern unsigned short DAT_8009c960;
extern short DAT_1f80019e;
extern void FUN_8011fa6c(void);
extern int FUN_80042c70(TObj *, TObj *);
extern void FUN_800e9f74(int, int, int, int);

static __inline__ short hit(TObj *a, TObj *b)
{
    unsigned short off;

    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return -1;
    if (b->animFrame & 1) off = U16(b, 0x6e) - U16(b, 0x6c);
    else off = U16(b, 0x6c);
    if ((unsigned short)((U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2)) + (off + U16(a, 0x6c))) > S16(b, 0x6e) + S16(a, 0x6e))
        return -1;
    if ((unsigned short)((U16(a, 0x16) - U16(b, 0x16)) + (U16(b, 0x70) + U16(a, 0x70))) > S16(a, 0x72) + S16(b, 0x72))
        return -1;
    return 1;
}

void FUN_80044d54(TObj *a, TObj *b)
{
    int t;

    if (DAT_8009c960 == 3 && b->subtype == 4) {
        FUN_8011fa6c();
        return;
    }
    if (hit(a, b) != -1) {
        b->w7a = a->animFrame & 1;
        t = FUN_80042c70(a, b);
        if (t != 0) {
            if (t == 1) {
                b->active = 3;
                b->b04 = 2;
                b->step = 0;
                b->state = 0;
            } else {
                FUN_800e9f74(500, b->a.p.whole, b->y.p.whole, b->b.p.whole);
                b->active = 2;
                b->animFrame = 1 - (a->animFrame & 1);
                b->b04 = 2;
                b->step = 2;
                b->state = 0;
            }
        }
        DAT_1f80019e = 0;
    }
}
