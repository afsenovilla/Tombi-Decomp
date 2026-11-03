// near-twin of PAL komaPigDefeated (tools/jp_autofix.py)
// VERSION SLPS_01144 x01 80132DD0 352
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern char MOVE_TABLE_2[];
extern B D_80130FD4[];
extern A *D_80133BF4[];
extern char D_8013CA30[];
extern void *D_800716F4;
extern void applyAnimVelocityX(TObj *, unsigned short);

void func_80132DD0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        { void *an = D_800716F4;
        o->movetab = D_8013CA30;
        o->velV = -0x400;
        o->b0a = 2;
        o->animFrame &= 1;
        o->anim = an;
        o->b0f -= 30; }
        { TObj *q;
        for (q = (TObj *)o->d90; q; q = (TObj *)q->d90) q->b04 = 3;
        for (q = (TObj *)o->d94; q; q = (TObj *)q->d94) q->b04 = 3; }
        break;
        { unsigned char *p; { B *t = D_80130FD4; p = t[((A *)o->anim)->w2].c; }
        o->box3 = p[1]; }
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
