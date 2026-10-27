// FUNC 800f91d8 680 I0NFO2
// MATCHING 800f91d8 680
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void topdownActorSetAnim(TObj *o, unsigned short i, short n);
extern void topdownActorSetDirAnim(TObj *o, int i, short n);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CEF0[];
extern TObj *D_8009F2E8;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern short PLAYER_Y;

static inline void startWalk(TObj *n)
{
    n->step = 2;
    n->state = 0;
    n->animFrame = n->b69 - 1;
    topdownActorSetDirAnim(n, 8, 0);
}

static inline void stopWalk(TObj *n)
{
    n->b68 = 0;
    n->b6a = 0;
    n->step = 0;
    n->state = 0;
    n->animFrame = 1;
    topdownActorSetDirAnim(n, 0, 0);
}

void func_800F91D8(TObj *o)
{
    TObj *n = D_8009F2E8;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        startWalk(n);
        PLAYER_SUBSTATE = 4;
        PLAYER_STEP = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        p = *(Pos6 *)&n->a;
        if (D_8009CDD5 == 0xFF) {
            S32(n, 0x90) = func_8002DCC8(2, 0x49, &p);
        } else {
            S32(n, 0x90) = func_8002DCC8(2, 0x37, &p);
        }
        o->state++;
        break;
    case 2:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            if (PLAYER_Y > n->y.p.whole) {
                topdownActorSetAnim(n, 6, 0);
                o->state++;
            } else if (D_8009CEF0[0] == 1) {
                topdownActorSetAnim(n, 4, 0);
                if (D_8009CDD5 == 0 || D_8009CDD5 == 0xFF) {
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
        n->h->raw += 0x10000;
        if (n->h->p.whole >= 0x147) {
            D_8009CEF0[0] = 2;
            n->h->p.whole = 0x146;
            topdownActorSetAnim(n, 1, 0);
            n->animFrame = 1;
            o->state = 9;
        }
        break;
    case 9:
        stopWalk(n);
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
