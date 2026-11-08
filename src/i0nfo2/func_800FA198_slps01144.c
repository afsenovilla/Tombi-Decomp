// VERSION SLPS_01144 info2 800FA198 608
// JP version of PAL i0nfo2 800f8f30 (npcSet07SceneStep4.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002E42C(int a, int b, Pos6 *p);
extern void topdownActorSetAnim(TObj *o, unsigned short i, short n);
extern void topdownActorSetDirAnim(TObj *o, int i, short n);
extern struct { unsigned char v; } D_8009E4E6;
extern struct { unsigned char v; } D_8009E4E7;
extern struct { unsigned char v; } D_8009E4EA;
extern unsigned char D_8009E97D;
extern unsigned char D_8009EA98[];
extern TObj *D_800A0E74;
extern struct { unsigned char v; } D_800A7B7C;
extern struct { unsigned char v; } D_800A7B7D;
extern struct { unsigned char v; } D_800A7B7E;
extern short PLAYER_Y;

static inline void startWalk(TObj *n)
{
    topdownActorSetAnim(n, 9, 0);
    n->step = 2;
    n->state = 0;
    n->animFrame = n->b69 - 1;
}

static inline void stopWalk(TObj *n)
{
    n->b68 = 0;
    n->b6a = 0;
    n->step = 0;
    n->state = 0;
}

void func_800FA198(TObj *o)
{
    TObj *n = D_800A0E74;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        startWalk(n);
        D_800A7B7D.v = 4;
        D_800A7B7E.v = 0;
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        o->state++;
        break;
    case 1:
        p = *(Pos6 *)&n->a;
        if (D_8009E97D == 0xFF) {
            S32(n, 0x90) = func_8002E42C(2, 0x48, &p);
        } else {
            S32(n, 0x90) = func_8002E42C(2, 0x36, &p);
        }
        o->state++;
        break;
    case 2:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            if (D_8009EA98[0] == 1) {
                topdownActorSetAnim(n, 6, 0);
                if (D_8009E97D == 0 || D_8009E97D == 0xFF) {
                    o->state++;
                } else {
                    topdownActorSetAnim(n, 1, 0);
                    o->state = 9;
                }
            } else {
                topdownActorSetAnim(n, 1, 0);
                o->state = 9;
            }
        }
        break;
    case 3:
        n->h->raw += 0xFFFF0000;
        if (n->h->p.whole < 0x10E) {
            D_8009EA98[0] = 2;
            n->h->p.whole = 0x10E;
            topdownActorSetAnim(n, 1, 0);
            n->animFrame = 1;
            o->state = 9;
        }
        break;
    case 9:
        stopWalk(n);
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
