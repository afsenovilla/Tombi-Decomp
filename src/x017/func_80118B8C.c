// FUNC 80118b8c 224 X017
// MATCHING 80118b8c 224
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned char D_8009CFD6;
extern TObj *D_8009C330;
extern void func_8011890C(TObj *);
extern void FUN_80018980(TObj *);

void func_80118B8C(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8011890C(o);
        o->b04++;
        break;
    case 1:
        if ((unsigned short)(D_800A6038.h->p.whole - 0x100) < 0x90 && D_8009CFD6 == 0)
            *((unsigned char *)D_8009C330 + 0x1e) = 0;
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
