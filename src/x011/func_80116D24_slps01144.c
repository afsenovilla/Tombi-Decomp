// VERSION SLPS_01144 x11 80116D24 516
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009E4EA, D_8009EE58;
extern short D_1F8001C6; extern volatile short D_800A7C2A;
#include "player.h"
extern void func_8001C88C(int, int, int);
extern int func_8002E42C(int, int, V6 *);

void func_80116D24_slps01144(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009E4EA = 1;
        D_8009EE58 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        *(int *)((char *)o + 0x90) = func_8002E42C(2, 0, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        *(int *)((char *)o + 0x90) = func_8002E42C(2, 1, &v);
        o->state++;
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        func_8001C88C(0x41, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 3:
        if (--o->timer != 0) break;
        *(int *)((char *)o + 0x90) = func_8002E42C(2, 2, &v);
        o->state++;
        break;
    case 4:
        if (((TObj *)o->d90)->b04 != 2) break;
        ((TObj *)o->d90)->b04 = 3;
        o->state++;
        break;
    case 5:
        D_8009E4EA = 0;
        D_800A7C2A = 0;
        { unsigned short w = *(volatile unsigned short *)&o->wbc;
        D_1F8001C6 = 0;
        o->animFrame = w; }
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
#endif
