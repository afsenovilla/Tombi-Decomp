// FUNC 8012dcfc 400 X000
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *o);
extern void FUN_8001fe6c(TObj *o);
extern unsigned FUN_8001f9e0(void);
extern short FUN_80040278(TObj *o, int x, int y);
extern void *PTR_8013b26c;
extern char D_80077cdc[];

void FUN_8012dcfc(TObj *o)
{
    short v;
    unsigned short u;
    switch (o->state) {
    case 0:
        o->active = 2;
        o->movetab = D_80077cdc;
        o->animFrame = FUN_8001f9e0() & 1;
        o->wac = 0;
        o->anim = PTR_8013b26c;
        if (o->b0c == 0) o->velY = -0x400;
        else o->velY = 0x80;
        v = o->h->p.whole;
        o->velV = o->y.p.whole;
        o->velH = v;
        FUN_8001fe6c(o);
        o->state = 1;
        break;
    case 1:
        FUN_8001fec0(o);
        v = o->velY;
        u = *(unsigned short *)&o->velY + 0x20;
        o->velY = u;
        o->y.raw = o->y.raw + v * 0x100;
        if ((short)u > 0) o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        o->y.raw = o->y.raw + o->velY * 0x100;
        v = o->y.p.whole;
        *(unsigned short *)&o->velY = *(unsigned short *)&o->velY + 0x20;
        if (FUN_80040278(o, o->h->p.whole, (short)(v + 0x10 + o->b0c * -0x10)) != 0) {
            if (o->w7a == 0) o->active = 1;
            o->step = 1;
            o->state = 0;
            o->b69 = 0;
        }
        break;
    }
}
