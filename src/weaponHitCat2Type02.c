// FUNC 80044d54 452 MAIN0
// MATCHING 80044d54 452
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern unsigned short DAT_8009c960;
extern short DAT_1f80019e;
extern void FUN_8011fa6c(void);
extern int resolveHitResultE(TObj *, TObj *);
extern void awardAPByArea(int, int, int, int);

static __inline__ short hit(TObj *a, TObj *b)
{
    unsigned short off;

    if ((unsigned short)(U16(a->d, 2) - U16(b->d, 2) + 0x2d) >= 0x5b)
        return -1;
    if (b->animFrame & 1) off = (*(unsigned short*)&b->box1) - (*(unsigned short*)&b->box0);
    else off = (*(unsigned short*)&b->box0);
    if ((unsigned short)((U16(a->h, 2) - U16(b->h, 2)) + (off + (*(unsigned short*)&a->box0))) > b->box1 + a->box1)
        return -1;
    if ((unsigned short)(((*(unsigned short*)&a->y.p.whole) - (*(unsigned short*)&b->y.p.whole)) + ((*(unsigned short*)&b->box2) + (*(unsigned short*)&a->box2))) > a->box3 + b->box3)
        return -1;
    return 1;
}

void weaponHitCat2Type02(TObj *a, TObj *b)
{
    int t;

    if (DAT_8009c960 == 3 && b->subtype == 4) {
        FUN_8011fa6c();
        return;
    }
    if (hit(a, b) != -1) {
        b->w7a = a->animFrame & 1;
        t = resolveHitResultE(a, b);
        if (t != 0) {
            if (t == 1) {
                b->active = 3;
                b->b04 = 2;
                b->step = 0;
                b->state = 0;
            } else {
                awardAPByArea(500, b->a.p.whole, b->y.p.whole, b->b.p.whole);
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
