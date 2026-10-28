// FUNC 801205a4 332 X003
// MATCHING 801205a4 332
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern B D_80135B30[];
int func_801205A4(TObj *o)
{
    A *a;
    unsigned char *p;
    int idx, dy, d;
    short sx;
    unsigned short v;
    if (--o->animTimer == 0) {
        a = o->anim;
        d = a->w6;
        switch (d & 0xc000) {
        case 0:
            o->anim = a + 1;
            idx = a[1].w2;
            goto merge;
        case 0x4000:
            o->anim = a + 1;
            o->anim = *(A **)(a + 1);
            idx = ((A *)o->anim)->w2;
        merge:
            { B *t = D_80135B30; p = t[idx].c; }
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p;
            o->box3 = p[1];
            o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
            v = ((A *)o->anim)->w4;
            d = v & 0xff;
            dy = v >> 8;
            sx = d;
            if (o->animFrame & 1) sx = -d;
            o->h->p.whole += sx;
            o->y.p.whole += dy;
            break;
        case 0x8000:
            o->animTimer = d & 0x3fff;
            return 1;
        case 0xc000:
            o->animTimer = d & 0x3fff;
            return 1;
        }
    }
    return 0;
}
