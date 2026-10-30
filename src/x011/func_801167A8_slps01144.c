// VERSION SLPS_01144 x11 801167A8 512
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009E4EA, D_8009EE58;
extern short D_1F8001C6; extern volatile short D_800A7C2A;
#include "player.h"
extern unsigned char D_8009E4E8, D_8009E4E9;
extern unsigned short D_800A7B8A, D_1F8001FC, D_1F8003C8;
typedef struct { short s0, s2; } S2;
extern S2 *D_800A7BBC;
extern int func_8002E42C(int, int, V6 *);

void func_801167A8_slps01144(TObj *o)
{
    V6 v;
    TObj *p;
    unsigned char *pp;

    switch (o->state) {
    case 0:
        pp = &D_8009E4E8;
        if (*pp != 0) {
            if (D_8009E4E9 == 0x41) {
                o->step = 3;
                o->state = 0;
            }
            *pp = 0;
        }
        if ((unsigned short)(D_800A7B8A - 0x40) >= 0x20) break;
        if (D_800A7BBC->s2 != 0) break;
        if (!(D_1F8001FC & D_1F8003C8)) break;
        v = *(V6 *)&o->a;
        D_8009E4EA = 1;
        D_8009EE58 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        *(int *)((char *)o + 0x90) = func_8002E42C(2, 0xa, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 2:
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
