// FUNC 800fa278 428 I0NFO2
// MATCHING 800fa278 428
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void topdownActorSetAnim(TObj *o, unsigned short i, short n);
extern void topdownActorSetDirAnim(TObj *o, int i, short n);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern TObj *D_8009F304;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern unsigned char D_800A60E4;

void func_800FA278(TObj *o)
{
    TObj *n = D_8009F304;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        n->step = 2;
        n->state = 0;
        D_800A603D = 4;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        n->animFrame = n->b69 - 1;
        topdownActorSetDirAnim(n, 8, 0);
        p = *(Pos6 *)&n->a;
        S32(n, 0x90) = func_8002DCC8(2, 0x35, &p);
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
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A60E4 = 0;
        n->animFrame = 3;
        topdownActorSetDirAnim(n, 0, 0);
        n->b68 = 0;
        n->b6a = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        n->step = 0;
        n->state = 0;
        break;
    }
}
