// FUNC 8011d9a4 316 X004
// MATCHING 8011d9a4 316
#include "TOBJ.H"
extern unsigned char D_8009C93F[];
extern unsigned char D_801152E8[];
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_8001e560(int, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_800eea3c(TObj *);

#define B(o, n) (((unsigned char *)(o))[n])

void func_8011D9A4(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C93F[0] = 1;
        o->wb2 = 0;
        B(o, 0xa2) = 1;
        PlayerSetAnimIfChanged(o, 0x41);
        FUN_8001e560(0x25, 0x28);
        B(o, 0xab) = 2;
        o->timer = 0x78;
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        if (--o->timer > 0) break;
        o->timer = 0x3c;
        o->state++;
        break;
    case 2:
        FUN_800eea3c(o);
        FUN_8001fec0(o);
        if (--o->timer > 0) break;
        D_8009C93F[0] = 0;
        o->d8c = D_801152E8[o->wb0];
        o->b04 = 1;
        o->wb2 = 0;
        B(o, 0xac) = 0;
        B(o, 0xab) = 0;
        o->step = 0x20;
        o->state = 0;
        break;
    }
}
