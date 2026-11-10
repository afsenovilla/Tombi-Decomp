// VERSION PCPX_96084 tomba 80079698 772
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x000 8012D9FC (chickWander.c): field +0xA8 (PAL +0xAC), frame counter at 0x1F8002AC, a three-argument sound call, depth -5.
#include "TOBJ.H"
extern char D_800A1898[];
extern void *D_80014050;
extern unsigned char D_800AEBF4[];
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int func_800180D8_pcpx96084(TObj *);
extern void applyFrameVelocityXY(TObj *);
extern short func_8001B9AC(TObj *, short, short);
extern void func_80022FC8_pcpx96084(int, int, int);
extern unsigned short D_1F8002AC;

void func_80079698_pcpx96084(TObj *o)
{
    short t;
    switch (o->state) {
    case 0:
        o->movetab = D_800A1898;
        o->animFrame = Rand() & 1;
        o->anim = D_80014050;
        AnimLoadDuration(o);
        switch (D_800AEBF4[Rand() & 0xf]) {
        case 0:
            o->timer = 60;
            *(short *)((char *)o + 0xa8) = 0;
            o->state = 1;
            break;
        case 1:
            *(short *)((char *)o + 0xa8) = 1;
            o->timer = 0x5a;
            o->state = 2;
            break;
        case 2:
            *(short *)((char *)o + 0xa8) = 2;
            o->velY = -0x200;
            o->timer = 0;
            o->state = 4;
            break;
        }
        break;
    case 1:
        if (--o->timer == 0)
            o->state++;
        func_800180D8_pcpx96084(o);
        break;
    case 2:
        o->anim = D_80014050;
        AnimLoadDuration(o);
        o->timer = 0x60;
        o->state++;
    case 3:
        func_800180D8_pcpx96084(o);
        applyFrameVelocityXY(o);
        if (o->velH + 8 < o->h->p.whole)
            o->h->p.whole = o->velH + 8;
        if (o->h->p.whole < o->velH - 8)
            o->h->p.whole = o->velH - 8;
        (t = o->y.p.whole + 0x10, func_8001B9AC(o, o->h->p.whole, t - o->b0c * 16));
        if ((D_1F8002AC & 0xf) == 0 && (Rand() & 3) == 0)
            func_80022FC8_pcpx96084(2, 0xe, 0);
        if (--o->timer == 0)
            o->state = 0;
        break;
    case 4:
        func_800180D8_pcpx96084(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state = 5;
        break;
    case 5:
        func_800180D8_pcpx96084(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if ((t = o->y.p.whole + 0x10, func_8001B9AC(o, o->h->p.whole, t - o->b0c * 16)))
            o->state = 0;
        break;
    case 6:
        *(signed char *)&o->b0f = -5;
        func_800180D8_pcpx96084(o);
        break;
    }
}
#endif
