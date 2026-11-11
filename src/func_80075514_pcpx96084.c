// VERSION PCPX_96084 tomba 80075514 344
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of komaPigDefeated (src/x004/komaPigDefeated.c): no draw/state setup in state 0, field +0xA8, halfword box table.
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned short c[4]; } B;
extern char D_800A18B0[];
extern B D_800AE9A4[];
extern A *D_80013460[];
extern void applyAnimVelocityX(TObj *, unsigned short);

void func_80075514_pcpx96084(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velV = -0x400;
        o->movetab = D_800A18B0;
        *(short *)((char *)o + 0xa8) = 0x26;
        o->state++;
        o->anim = D_80013460[0];
        { unsigned short *p; { B *t = D_800AE9A4; p = t[((A *)o->anim)->w2].c; }
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1]; }
        o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
        break;
    case 1:
        applyAnimVelocityX(o, 1 - o->animFrame);
        o->velV += 0x40;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        break;
    }
    if (o->animFrame & 1)
        o->d8c = (o->d8c + 0x14) & 0xff;
    else
        o->d8c = (o->d8c - 0x14) & 0xff;
}
#endif
