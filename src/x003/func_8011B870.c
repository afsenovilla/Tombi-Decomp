// FUNC 8011b870 452 X003
// MATCHING 8011b870 452
#include "TOBJ.H"

extern void *D_80139864[];
extern unsigned short D_8007A1F0[];
extern unsigned char D_800A6038[], D_800A603C[], D_800A603D[], D_800A603E[];
extern signed char D_8009D2B0[];
extern unsigned short D_1F800172, D_1F80016A, D_1F80016E;
extern void AnimLoadDuration(TObj *);
extern void func_8011B588(TObj *, int, int);
extern int func_8011B460(TObj *);

static __inline__ int near(TObj *o)
{
    unsigned short t;
    short lim = 0xc8;
    if (D_800A6038[0] == 2) return 0;
    t = D_1F800172 - o->d->p.whole + 0x2d;
    if (t >= 0x5b) return 0;
    t = D_1F80016A - o->h->p.whole + 0x64;
    if (t > lim) return 0;
    t = D_1F80016E - o->y.p.whole + 0x78;
    return t <= lim - 0x10;
}

void func_8011B870(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->wac = 0;
        o->anim = D_80139864[0];
        AnimLoadDuration(o);
        break;
    case 1:
        o->ba7 += 2;
        if (o->ba7 > 0x80) o->ba7 = 0;
        { short t = (short)D_8007A1F0[o->ba7] >> 6; o->ba6 = t; }
        func_8011B588(o, 0, 0);
        if (func_8011B460(o)) {
            o->b0a = 2;
            o->velH = 0x80;
            D_800A603C[0] = 1;
            D_8009D2B0[0] = 0;
            D_800A6038[0] = 2;
            D_800A603D[0] = 0x36;
            D_800A603E[0] = 0;
            o->state = 0;
            o->step++;
        } else if (!near(o)) {
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
