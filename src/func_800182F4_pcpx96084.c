// VERSION PCPX_96084 tomba 800182F4 264
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// AnimAdvance (src/AnimAdvance.c) that also moves the object by the frame's offset (x mirrored by facing).
#include "TOBJ.H"
typedef struct AE { char p0[4]; unsigned short m; unsigned short v; } AE;

int func_800182F4_pcpx96084(TObj *o)
{
    AE *e, *n; unsigned short v, w; int k;
    short dx; unsigned short m; int dy;
    o->animTimer = o->animTimer - 1;
    if (o->animTimer != 0) return 0;
    e = o->anim;
    v = e->v;
    k = v & 0xc000;
    switch (k) {
    case 0:
        o->anim = e + 1;
        w = e[1].v;
        goto tail;
    case 0x4000:
        o->anim = e + 1;
        n = *(AE **)(e + 1);
        o->anim = n;
        w = n->v;
    tail:
        o->animTimer = w & 0x3fff;
        m = *(volatile unsigned short *)&((AE *)o->anim)->m;
        { short d = m & 0xff;
        dx = d; asm("" : "=r"(dx) : "0"(dx));
        dy = m >> 8;
        if (*(volatile unsigned short *)&o->animFrame & 1) dx = -d; }
        o->h->p.whole += dx;
        o->y.p.whole += dy;
        break;
    case 0x8000:
    case 0xc000:
        o->animTimer = v & 0x3fff;
        return 1;
    }
    return 0;
}

#endif
