// FUNC 80044b80 468 MAIN0
// MATCHING 80044b80 468
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern unsigned short DAT_1f80019e;
extern void FUN_80042b98(TObj *, TObj *);
extern TObj *FUN_80018448(void);

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

static __inline__ int spawn(TObj *a, TObj *b)
{
    TObj *o = FUN_80018448();

    if (o) {
        o->active = 2;
        o->type = 0x5b;
        o->d90 = (int)a;
        o->d94 = (int)b;
        b->active = 5;
        b->h->p.whole = a->h->p.whole + (DAT_1f80019e & 3) * 4;
        b->y.p.whole = a->y.p.whole;
        b->d->p.whole = a->d->p.whole;
        return 1;
    }
    return 0;
}

void FUN_80044b80(TObj *a, TObj *b)
{
    unsigned short v, f;
    short t;

    if (hit(a, b) >= 0) {
        FUN_80042b98(a, b);
        t = a->type;
        if ((unsigned char)t == 10)
            return;
        if ((unsigned)(t - 5) < 3 && spawn(a, b)) {
        } else {
            b->b68 = 1;
            if (a->type == 1) {
                v = a->animFrame & 1;
            } else {
                f = a->animFrame;
                if (f < 6)
                    v = f & 1;
                else
                    v = 2;
            }
            b->animFrame = v;
        }
        DAT_1f80019e = 0;
    }
}
