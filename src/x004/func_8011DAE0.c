// FUNC 8011dae0 300 X004
// MATCHING 8011dae0 300
#include "TOBJ.H"

extern unsigned char D_801152E8[];
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void playSFX(int);
extern int AnimAdvance(TObj *);
extern void func_800EEA3C(TObj *);

void func_8011DAE0(TObj *o)
{
    unsigned char one;

    switch (o->state) {
    case 0:
        one = 1;
        o->wb2 = 0;
        ((unsigned char *)o)[0xa2] = one;
        PlayerSetAnimIfChanged(o, 0x42);
        playSFX(0x20);
        ((unsigned char *)o)[0xab] = one;
        o->timer = 0x78;
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        if (--o->timer > 0) break;
        o->timer = 0x3c;
        o->state++;
        break;
    case 2:
        func_800EEA3C(o);
        AnimAdvance(o);
        if (--o->timer > 0) break;
        o->wb2 = 0;
        *(unsigned char *)&o->wac = 0;
        ((unsigned char *)o)[0xab] = 0;
        o->d8c = D_801152E8[o->wb0];
        o->b04 = 1;
        o->step = 0x20;
        o->state = 0;
        break;
    }
}
