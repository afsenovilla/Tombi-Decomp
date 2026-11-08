// VERSION SLPS_01144 info2 800FA83C 656
// JP version of PAL i0nfo2 800f969c (npcSet07SceneStep12.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
typedef struct { unsigned char v; } U8S;
extern void func_80027A5C(int a, int b, int c);
extern TObj *func_8002E42C(int a, int b, Pos6 *p);
extern void func_8001C988(int a, int b);
extern void topdownActorSetAnim(TObj *o, unsigned short i, short n);
extern U8S D_8009E4E6;
extern U8S D_8009E4E7;
extern U8S D_8009E4EA;
extern TObj *D_800A0E98;
extern U8S D_800A7B7C;
extern U8S D_800A7B7D;
extern U8S D_800A7B7E;
extern struct { unsigned short v; } D_800A7BA6;

void func_800FA83C(TObj *o)
{
    Pos6 p;
    TObj *n = D_800A0E98;
    TObj *m;
    switch (o->state) {
    case 0:
        o->state++;
    case 1:
        topdownActorSetAnim(n, 8, 0);
        n->step = 2;
        n->state = 0;
        n->animFrame = n->b69 - 1;
        D_800A7B7D.v = 4;
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        D_800A7B7E.v = 0;
        D_800A7BA6.v = 4;
        o->state++;
        break;
    case 2:
        p = *(Pos6 *)&n->a;
        n->d90 = (int)func_8002E42C(2, 0x21, &p);
        o->state++;
    case 3:
        m = (TObj *)n->d90;
        if (m->b04 == 2) {
            m->b04 = 3;
            func_8001C988(0xA7, 1);
            func_80027A5C(0x68, 1, 1);
            topdownActorSetAnim(n, 0, 0);
            o->w08 = 0xC8;
            o->state++;
        }
        break;
    case 4:
        if (--o->w08 <= 0) {
            o->w08 = 0x20;
            topdownActorSetAnim(n, 6, 0);
            o->state++;
        }
        break;
    case 5:
        n->h->p.whole--;
        if (--o->w08 <= 0) {
            topdownActorSetAnim(n, 7, 0);
            o->w08 = 0xE6;
            o->state++;
        }
        break;
    case 6:
        n->y.p.whole++;
        if (--o->w08 <= 0) {
            n->b04 = 3;
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
