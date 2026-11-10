// VERSION PCPX_96084 tomba 80070958 332
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x000 80127380 (komaPigAnimAdvance.c): the box table holds halfwords here.
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned short c[4]; } B;
extern B D_800AE9A4[];
int func_80070958_pcpx96084(TObj *o)
{
    A *a;
    unsigned short *p;
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
            { B *t = D_800AE9A4; p = t[idx].c; }
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
#endif
