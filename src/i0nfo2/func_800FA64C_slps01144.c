// VERSION SLPS_01144 info2 800FA64C 496
// JP version of PAL i0nfo2 800f9480 (npcSet07SceneStep11.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002E42C(int a, int b, Pos6 *p);
extern void topdownActorSetAnim(TObj *o, unsigned short i, short n);
extern void func_8001C988(int e, int a);
extern struct { unsigned char v; } D_8009E4E6;
extern struct { unsigned char v; } D_8009E4E7;
extern struct { unsigned char v; } D_8009E4EA;
extern TObj *D_800A0E98;
extern struct { unsigned char v; } D_800A7B7C;
extern struct { unsigned char v; } D_800A7B7D;
extern struct { unsigned char v; } D_800A7B7E;
extern struct { unsigned short v; } D_800A7BA6;

void func_800FA64C(TObj *o)
{
    TObj *n = D_800A0E98;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        o->state++;
    case 1:
        topdownActorSetAnim(n, 11, 0);
        n->step = 2;
        n->state = 0;
        D_800A7B7D.v = 4;
        D_800A7B7E.v = 0;
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        n->animFrame = 3;
        D_800A7BA6.v = 2;
        o->state++;
        break;
    case 2:
        topdownActorSetAnim(n, 11, 0);
        p = *(Pos6 *)&n->a;
        S32(n, 0x90) = func_8002E42C(2, 0x1F, &p);
        o->state++;
    case 3:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            func_8001C988(0x2B, 1);
            topdownActorSetAnim(n, 3, 0);
            o->w08 = 0xC8;
            o->state = 8;
        }
        break;
    case 8:
        if (--o->w08 <= 0) {
            o->state = 9;
        }
        break;
    case 9:
        n->b68 = 0;
        n->b6a = 0;
        n->step = 0;
        n->state = 0;
        D_8009E4E7.v = 0;
        D_8009E4E6.v = 0;
        D_8009E4EA.v = 0;
        D_800A7B7C.v = 1;
        D_800A7B7D.v = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
#endif
