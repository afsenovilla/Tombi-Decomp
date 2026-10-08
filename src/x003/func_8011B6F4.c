// FUNC 8011b6f4 380 X003
// MATCHING 8011b6f4 380
#include "TOBJ.H"
extern unsigned char D_800A6038;
extern unsigned short D_1F800172, D_1F80016A, D_1F80016E;
extern void *D_80139864[];
extern unsigned short D_8007A1F0[];
extern void FUN_8001fe6c(TObj *);
extern void func_8011B588(TObj *, int, int);

static __inline__ int near(TObj *o, short x, short w)
{
    int d;

    if (D_800A6038 == 2) return 0;
    d = D_1F800172 - (unsigned short)o->d->p.whole + 0x2d;
    if ((unsigned short)d >= 0x5b) return 0;
    d = D_1F80016A - (unsigned short)o->h->p.whole + x;
    if ((d & 0xffff) > w) return 0;
    d = D_1F80016E - (unsigned short)o->y.p.whole + 0x6e;
    return (d & 0xffff) < 0xaf;
}

void func_8011B6F4(TObj *o)
{
    switch (o->state) {
    case 0:
        o->ba7 = 0;
        o->b0a = 7;
        o->wac = 0;
        o->state++;
        o->anim = D_80139864[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        func_8011B588(o, 1, 0);
        if (near(o, 0x60, 0xc0)) {
            o->state = 0;
            o->step++;
        }
        o->ba7 += 2;
        if (o->ba7 > 0x80) o->ba7 = 0;
        { short v = (short)D_8007A1F0[o->ba7] >> 6; o->ba6 = v; }
        break;
    }
}
