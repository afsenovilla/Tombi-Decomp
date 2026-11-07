// VERSION SLPS_01144 x09 8011C2D8 428
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"


extern void *D_8012ADB8[];
void func_80018378(TObj *o);
void func_80017F08(TObj *o);

void func_8011C2D8_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int x, d;
        o->w1e = 9;
        o->b0d = 0;
        o->anim = D_8012ADB8[o->b0c];
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
            func_80018378(o);
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
                func_80018378(o);
            }
            break;
        }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80017F08(o);
        break;
    }
}
#endif
