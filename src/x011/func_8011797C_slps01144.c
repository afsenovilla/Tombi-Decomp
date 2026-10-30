// VERSION SLPS_01144 x11 8011797C 384
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009E4EA, D_8009EE58;
extern short D_1F8001C6; extern volatile short D_800A7C2A;
#include "player.h"
extern int func_8002E42C(int, int, V6 *);

void func_8011797C_slps01144(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        D_8009E4EA = 1;
        D_8009EE58 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(V6 *)&o->a;
        *(int *)((char *)o + 0x90) = func_8002E42C(2, 0x12, &v);
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
