// FUNC 8011ca60 320 X014
// MATCHING 8011ca60 320
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern unsigned short D_8009C962;
extern unsigned char *D_8009C330[];
extern short FUN_80042fbc(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_8011CA60(TObj *a, TObj *b)
{
    short v;

    if (*(unsigned char *)&a->wac == 2) return;
    if (a->active & 2) return;
    if (!FUN_80042fbc(a, b)) return;
    if (D_1F8001A4) return;
    if (D_8009C962 == 7) {
        a->active = 2;
        a->animFrame = b->h->p.whole > a->h->p.whole;
        a->b04 = 2;
        a->step = 0;
        a->state = 0;
        FUN_8004258c(a, 1);
    } else {
        if (a->h->p.whole > b->h->p.whole) {
            a->animFrame = 1;
            v = 0x200;
        } else {
            v = -0x200;
            a->animFrame = 0;
        }
        a->velX = v;
        D_8009C330[0][9] = 10;
        a->b04 = 6;
        a->step = 7;
        a->state = 0;
    }
    b->active = 2;
    b->b04 = 2;
    b->step = 0;
    b->state = 0;
    D_1F80019E = 0;
}
