// FUNC 8011bd5c 336 X010
// MATCHING 8011bd5c 336
#include "TOBJ.H"

extern unsigned short D_1F8001FC, D_1F8003C6, D_1F8003C4;
extern unsigned char D_8009D2B0, D_8009D2B2;
extern TObj *D_8009F0EC;
extern int D_8009C984;
extern unsigned short D_8009D670[];
extern TObj *D_8009C330;

void func_8011BD5C(TObj *o)
{
    int e;
    Fix16 *h;

    if (D_1F8001FC & D_1F8003C6) {
        D_8009D2B0 = 0;
        ((unsigned char *)&o->wa8)[1] = 0;
        if (D_8009D2B2 - 5 < 3) D_8009F0EC->active = 1;
        o->b9c = 1;
        o->timer = 10;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        if (D_8009F0EC->d94) {
            e = D_8009F0EC->d94;
            if (!((TObj *)e)->d94) {
                ((TObj *)e)->active = 1;
            } else {
            loop:
                ((TObj *)e)->active = 3;
                e = ((TObj *)e)->d94;
                if (((TObj *)e)->d94) goto loop;
                ((TObj *)e)->active = 1;
            }
        }
        h = o->h;
        e = h->p.whole;
        if (o->animFrame & 1) h->p.whole = e + 0x10; else h->p.whole = e - 0x10;
        if (D_8009C984 & 0x40) {
            if (*(volatile unsigned short *)D_8009D670 & D_1F8003C4) o->ba7 = 1;
        }
        D_8009C330->b04 = 0;
        o->step = 2;
        o->state = 0;
    }
}
