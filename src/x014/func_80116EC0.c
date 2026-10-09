// FUNC 80116ec0 476 X014
// MATCHING 80116ec0 476
#include "TOBJ.H"

extern unsigned short D_8009C962[];
extern unsigned short D_80125B70[];
extern void *D_80125C00[];
extern char D_8012A1B0[];
extern int D_1F8002DC;
extern void FUN_8001fe6c(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_800187e4(TObj *);

void func_80116EC0(TObj *o)
{
    unsigned char t = o->b04;
    TObj *p;
    signed char *q;

    switch (t) {
    case 0:
        o->w1e = 1;
        o->b0d = 0x81;
        o->w08 = (D_80125B70[D_8009C962[0]] << 6) | 0xb;
        o->animFrame = 0;
        o->anim = D_8012A1B0;
        *(void **)((char *)o + 0x94) = D_80125C00[D_8009C962[0]];
        o->d3c = D_1F8002DC;
        o->b04++;
        FUN_8001fe6c(o);
        o->timer = 9;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (--o->timer == -1) o->step++;
            break;
        case 1:
            p = (TObj *)o->d90;
            q = (signed char *)o->d94 + (p->wb8 & 0xe);
            o->a.p.whole = p->a.p.whole + q[0];
            o->y.p.whole = p->y.p.whole + q[1];
            if (AnimAdvance(o)) o->b04++;
            ObjCullRegister(o);
            break;
        }
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
