// FUNC 8011dff8 348 X004
// MATCHING 8011dff8 348
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_8013B104[];
extern unsigned char D_8009D2AE;
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_8011DE60(TObj *);
extern void FUN_8001e4f0(int);
extern void FUN_800ea544(TObj *, short, short, short);
extern void FUN_80018744(TObj *);

void func_8011DFF8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 0xa;
        o->box1 = 0x14;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->d3c = D_1F8002D4[0];
        o->b0a = 2;
        o->active = 2;
        o->w1e = 0xa;
        o->b0d = 0;
        o->d8c = 0;
        o->ba5 = 1;
        o->anim = D_8013B104[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o)) func_8011DE60(o);
        break;
    case 2:
        if (FUN_800202b4(o)) {
            FUN_8001e4f0(0x34);
            FUN_800ea544(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            { unsigned char *c = &D_8009D2AE; *c |= 1 << o->subtype; }
            o->b04 = 3;
        }
        break;
    case 3:
        FUN_80018744(o);
        break;
    }
}
