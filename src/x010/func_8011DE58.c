// FUNC 8011de58 144 X010
// MATCHING 8011de58 144
#include "TOBJ.H"
typedef struct {
    char p00[0x12]; short x; char p14[2]; short y; char p18[2]; short z;
    char p1c[0xac - 0x1c]; unsigned char bac;
    char pad[0xe4 - 0xad]; TObj *de4;
} PL;
extern short func_800435E0(PL *, TObj *);
extern void FUN_8001f96c(int, int, int, int);

void func_8011DE58(PL *p, TObj *o)
{
    short r = func_800435E0(p, o);
    if (r != -1 && r < 3 && p->bac == 1) {
        o->active = 2;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->b69 = 0;
        p->de4 = o;
        p->bac = 2;
        FUN_8001f96c(2, p->x, p->y, p->z);
    }
}
