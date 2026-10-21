// FUNC 800fd688 552 I0NFO2
// MATCHING 800fd688 552
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void topdownA17ActorSetDirAnim(TObj *o, int i, short n);
extern void func_80026C50(int item, int a, int b);
extern void func_8005A9A4(int e, int a);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern unsigned char D_800A60E4;

void func_800FD688(TObj *o)
{
    TObj *n; /* never set (original bug, needed to match) */
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        p = *(Pos6 *)&n->a;
        S32(n, 0x90) = func_8002DCC8(2, 7, &p);
        topdownA17ActorSetDirAnim(n, 8, 0);
        o->state++;
        D_800A603D = 2;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        break;
    case 1:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            n->animFrame = 1;
            topdownA17ActorSetDirAnim(n, 8, 0);
            o->w08 = 0x3C;
            o->state++;
        }
        break;
    case 2:
        if (--o->w08 <= 0) {
            S32(n, 0x90) = func_8002DCC8(2, 8, &p);
            n->animFrame = 3;
            topdownA17ActorSetDirAnim(n, 8, 0);
            o->state++;
        }
        break;
    case 3:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            func_80026C50(0x3E, 1, 1);
            topdownA17ActorSetDirAnim(n, 0, 0);
            o->state++;
            func_8005A9A4(0x62, 1);
        }
        break;
    case 4:
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A60E4 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
