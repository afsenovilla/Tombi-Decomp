// VERSION SLPS_01144 x11 80117AFC 524
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short v[6]; } V6;

extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);
extern unsigned char D_8009E4EA, D_8009EE58, D_800A7B7C, D_800A7B7D, D_800A7B7E;
extern unsigned short D_800A7C2A;
extern short DRAW_FROZEN;

void func_80117AFC_slps01144(TObj *o)
{
    TObj *p;
    V6 v;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        D_8009E4EA = 1;
        D_8009EE58 = 2;
        D_800A7B7C = 5;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 4, &v);
        o->state++;
        break;
    case 1:
        if (((TObj *)o->d90)->b04 != 2) break;
        ((TObj *)o->d90)->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 5, &v);
        o->state++;
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setEventStarted(79, 0, 0);
        o->timer = 360;
        o->state++;
        break;
    case 3:
        if (--o->timer != 0) break;
        o->state++;
        break;
    case 4:
        D_8009E4EA = 0;
        D_800A7C2A = 0;
        asm("");
        DRAW_FROZEN = 0;
        o->animFrame = o->wbc;
        asm("");
        D_800A7B7C = 1;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
#endif
