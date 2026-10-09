// FUNC 801210e8 440 X001
/* score 74: case 0 store/load scheduling (anim store and a.whole load placed early in game) and case 1 register choice (v1/a0 vs a1) differ; tried statement hill-climb (71 with odd orders), raw anim store, D_1F8002D4 as array, var/type forms for the clamp. */
#include "TOBJ.H"

extern void *D_8013E6B0[];
extern int D_1F8002D4;
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
void func_80018DA4(TObj *o);
void FUN_80018934(TObj *o);

void func_801210E8(TObj *o)
{

    switch (o->b04) {
    case 0:
        o->w1e = 0xb;
        o->y.p.whole += 0x180;
        o->b0d = 0;
        o->anim = D_8013E6B0[o->b0c];
        o->b0f = 0;
        o->d30 = o->a.p.whole;
        o->b.p.whole = 0;
        o->b04++;
        o->d34 = o->y.p.whole;
        o->d38 = 0x840;
        o->d3c = D_1F8002D4;
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            o->visible = 1;
            func_80018DA4(o);
            break;
        case 1: {
            int t; short u;
            t = o->d30; t -= D_1F800176;
            if ((short)t < -0x100) {
                t = (t & 0xfff) >> 3;
            } else {
                t = (short)t >> 3;
            }
            if ((short)t < 0x160) {
                o->a.p.whole = t;
                u = o->d34 - D_1F800186; if ((short)u < o->d38) t = o->d38; else t = u;
                o->y.p.whole = (t & 0xfff) >> 4;
                o->visible = 1;
                func_80018DA4(o);
            }
            break;
        }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
