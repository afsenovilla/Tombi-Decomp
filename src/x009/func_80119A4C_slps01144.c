// VERSION SLPS_01144 x09 80119A4C 548
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *o);
extern void AnimAdvance(TObj *o);
extern void ObjCullRegister(TObj *o);
extern void ObjFree(TObj *o);
extern int func_8011990C_slps01144(TObj *o);
extern void *D_8012ADDC;
extern int D_1f8002d4[];
extern unsigned char D_8009E4E6, D_8009E4E7, D_8009E4E8, D_8009E4E9, D_8009E4EA;

void func_80119A4C_slps01144(TObj *o)
{
    TObj *p;
    switch (o->b04) {
    case 0:
        o->b04++;
        *(signed char *)&o->b0f = -11;
        o->w1e = 11;
        o->b0d = 0;
        o->d3c = D_1f8002d4[0];
        asm("");
        o->anim = D_8012ADDC;
        AnimLoadDuration(o);
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (D_8009E4E8 && D_8009E4E9 == 117 && func_8011990C_slps01144(o)) {
                o->timer = 30;
                o->step++;
            }
            break;
        case 1:
            p = (TObj *)o->d90;
            o->a.p.whole = p->a.p.whole;
            o->y.p.whole = p->y.p.whole - 16;
            o->b.p.whole = p->b.p.whole;
            AnimAdvance(o);
            if (--o->timer == -1) {
                o->step++;
                p->step = 8;
                p->state = 0;
                p->subtype |= 0x80;
                o->timer = 100;
            }
            ObjCullRegister(o);
            break;
        case 2:
            AnimAdvance(o);
            if (--o->timer == -1) {
                o->step = 0;
                D_8009E4E6 = 0;
                D_8009E4EA = 0;
                D_8009E4E7 = 0;
                D_8009E4E8 = 0;
            }
            ObjCullRegister(o);
            break;
        }
        break;
    case 2:
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
