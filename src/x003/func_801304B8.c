// FUNC 801304b8 396 X003
// MATCHING 801304b8 396
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009D00A[];
extern void *D_8013A3B4[];
extern void *D_8013A368[];
extern TObj *FUN_8002dcc8(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void func_801304B8(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            o->d90 = (int)FUN_8002dcc8(2, 0xd, &o->a);
            o->wac = 3;
            o->animFrame = o->b68 & 1;
            o->anim = D_8013A3B4[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            o->wac = 0;
            o->anim = D_8013A368[0];
            o->state = 0;
            if (D_8009D00A[0]) {
                D_8009C93F = 0;
                D_8009C942 = 0;
                D_800A603C = 1;
                D_800A603D = 0;
                D_800A603E = 0;
            } else {
                D_8009D00A[0] = 1;
            }
        }
        break;
    }
}
