// VERSION SCUS_94236 x09 8011C4CC 428
// FLAGS.SCUS_94236 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/x009/func_8011C2D8_slps01144.c (func_8011C2D8_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#include "spad.h"
#include "TOBJ.H"


extern void *D_8012D3E8[];
void func_80018D40(TObj *o);
void func_800188D0(TObj *o);

void func_8011C4CC_scus94236(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int x, d;
        o->w1e = 9;
        o->b0d = 0;
        o->anim = D_8012D3E8[o->b0c];
        x = o->a.p.whole;
        d = ((int*)&SPAD.i2d4)[0];
        o->b0f = 0;
        o->d30 = x;
        o->b.p.whole = 0;
        o->d34 = o->y.p.whole;
        o->d38 = 2432;
        o->b04++;
        o->d3c = d;
    }
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            o->visible = 1;
            func_80018D40(o);
            break;
        case 1: {
            short t; unsigned int u;
            u = o->d30; u -= (*(unsigned short*)((char*)&SPAD.i174 + 2)); t = u;
            if (t < -0x100) {
                t = (u & 0xfff) >> 3;
            } else {
                t = t >> 3;
            }
            if ((short)t < 0x160) {
                o->a.p.whole = t;
                t = o->d34 - ((unsigned short*)&SPAD.s186)[0];
                if ((short)t < o->d38) t = o->d38;
                t = (t & 0xfff) >> 4; o->y.p.whole = t;
                o->visible = 1;
                func_80018D40(o);
            }
            break;
        }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_800188D0(o);
        break;
    }
}
#endif
