// FUNC 801222c4 332 X001
// MATCHING 801222c4 332
#include "TOBJ.H"
typedef struct L { unsigned char active; char p[0x94 - 1]; struct L *next; } L;
extern TObj *D_8009C330;
extern L *D_8009F0EC;
extern unsigned short D_1F8001FC, D_1F8003C6, D_1F8003C4;
extern unsigned char D_8009D2B0, D_8009D2B2;
extern unsigned int D_8009C984[];
extern volatile unsigned short D_8009D670[];

void func_801222C4(TObj *o)
{
    L *q, *n;

    if (!(D_1F8001FC & D_1F8003C6)) return;
    D_8009D2B0 = 0;
    ((unsigned char *)&o->wa8)[1] = 0;
    if (D_8009D2B2 - 5 < 3) D_8009F0EC->active = 1;
    D_8009C330->b04 = 0;
    o->b9c = 1;
    o->timer = 10;
    o->velH = 0;
    o->velV = 0;
    o->velX = 0;
    o->velY = 0;
    n = D_8009F0EC->next;
    if (n != 0) {
        q = n;
        if (q->next == 0) {
            q->active = 1;
        } else {
        loop:
            q->active = 3;
            q = q->next;
            if (q->next != 0) goto loop;
            q->active = 1;
        }
    }
    o->h->p.whole += (o->animFrame & 1) ? 0x10 : -0x10;
    if ((D_8009C984[0] & 0x40) && (D_8009D670[0] & D_1F8003C4)) o->ba7 = 1;
    o->step = 2;
    o->state = 0;
}
