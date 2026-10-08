// FUNC 80127380 332 X000
// score 35: game computes la base before idx<<2 at the merge (no sll in j delay slots)
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern B D_80138FD8[];
int func_80127380(TObj *o)
{
    A *a;
    unsigned char *p;
    int idx, dx, dy;
    unsigned short v;
    if (--o->animTimer == 0) {
        a = o->anim;
        switch (a->w6 & 0xc000) {
        case 0:
            o->anim = a + 1;
            idx = a[1].w2;
            break;
        case 0x4000:
            o->anim = a + 1;
            o->anim = *(A **)(a + 1);
            idx = ((A *)o->anim)->w2;
            break;
        case 0x8000:
            o->animTimer = a->w6 & 0x3fff;
            return 1;
        case 0xc000:
            o->animTimer = a->w6 & 0x3fff;
            return 1;
        default:
            return 0;
        }
        p = D_80138FD8[idx].c;
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
        v = ((A *)o->anim)->w4;
        dx = v & 0xff;
        dy = v >> 8;
        if (o->animFrame & 1) dx = -dx;
        o->h->p.whole += dx;
        o->y.p.whole += dy;
    }
    return 0;
}
