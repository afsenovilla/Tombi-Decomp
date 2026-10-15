// FUNC 800449a0 480 MAIN0
// MATCHING 800449a0 480
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern unsigned short DAT_1f80019e;
extern int weaponOnHitGetResult(TObj *, TObj *);
extern void spawnImpactEffect(int, int, int, int);
extern void SfxPlay(int);

static __inline__ short hit(TObj *a, TObj *b)
{
    short dx;
    if ((unsigned short)(U16(a->d, 2) - U16(b->d, 2) + 0x2d) >= 0x5b)
        return -1;
    dx = U16(a->h, 2) - U16(b->h, 2);
    if ((unsigned short)(dx + ((*(unsigned short*)&b->box0) + (*(unsigned short*)&a->box0))) > b->box1 + a->box1)
        return -1;
    if ((unsigned short)(((*(unsigned short*)&a->y.p.whole) - (*(unsigned short*)&b->y.p.whole)) + ((*(unsigned short*)&a->box2) + (*(unsigned short*)&b->box2))) > a->box3 + b->box3)
        return -1;
    DAT_1f80019e = 0;
    if (dx < 0)
        return 0;
    return 1;
}

static __inline__ int react(TObj *a, TObj *b)
{
    int r = 1;
    b->b68 = 1;
    b->b9e = 0;
    switch (weaponOnHitGetResult(a, b)) {
    case 1: case 4: case 5: case 7: case 10:
        r = 1;
        break;
    case 13:
        if ((b->category & 0x7f) == 4) {
            b->b68 = 0;
            return r;
        }
    case 0: case 3: case 6: case 9: case 11: case 12:
        r = 2;
        break;
    case 2: case 8:
        r = 0;
        b->b68 = 0;
        b->b9e = 1;
        b->b9f = 0;
        break;
    }
    spawnImpactEffect(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
    SfxPlay((b->category & 0x7f) == 4 ? 6 : 7);
    return r;
}

void FUN_800449a0(TObj *a, TObj *b)
{
    short h;

    h = hit(a, b);
    if (h < 0)
        return;
    if (react(a, b) && a->type != 10) {
        short t = a->w98;
        b->animFrame = h & 1;
        b->b68 = t;
    }
}
