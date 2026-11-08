// VERSION SLPS_01144 info2 800FB3D0 408
// JP version of PAL i0nfo2 800fa278 (npcSet07SceneStep10.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
typedef struct { unsigned char v; } U8S;
extern int func_8002E42C(int a, int b, Pos6 *p);
extern void topdownActorSetAnim(TObj *o, unsigned short i, short n);
extern U8S D_8009E4E6;
extern U8S D_8009E4E7;
extern U8S D_8009E4EA;
extern TObj *D_800A0E94;
extern U8S D_800A7B7C;
extern U8S D_800A7B7D;
extern U8S D_800A7B7E;
extern U8S D_800A7C24;

void func_800FB3D0(TObj *o)
{
    TObj *n = D_800A0E94;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        topdownActorSetAnim(n, 8, 3);
        n->step = 2;
        n->state = 0;
        D_800A7B7D.v = 4;
        D_800A7B7E.v = 0;
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        n->animFrame = n->b69 - 1;
        p = *(Pos6 *)&n->a;
        S32(n, 0x90) = func_8002E42C(2, 0x35, &p);
        o->state++;
        break;
    case 1:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            topdownActorSetAnim(n, 3, 0);
            o->state++;
        }
        break;
    case 2:
        o->state = 0;
        o->step = 0;
        D_8009E4E7.v = 0;
        D_8009E4E6.v = 0;
        D_8009E4EA.v = 0;
        D_800A7C24.v = 0;
        n->b68 = 0;
        n->b6a = 0;
        D_800A7B7C.v = 1;
        D_800A7B7D.v = 0;
        D_800A7B7E.v = 0;
        n->step = 0;
        n->state = 0;
        break;
    }
}
#endif
