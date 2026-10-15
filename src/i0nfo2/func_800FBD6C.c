// FUNC 800fbd6c 496 I0NFO2
// MATCHING 800fbd6c 496
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void func_800F2D50(TObj *o, int i, short n);
extern void func_8005A8A8(int e, int a, int b);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern TObj *D_8009F2F8;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern unsigned char D_800A60E4;

void func_800FBD6C(TObj *o)
{
    TObj *n;
    Pos6 p = *(Pos6 *)&n->a; /* reads n before it is set (original bug, needed to match) */
    TObj *b;
    n = D_8009F2F8;
    switch (o->state) {
    case 0:
        n->step = 2;
        n->state = 0;
        D_800A603D = 2;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        S32(n, 0x90) = func_8002DCC8(2, 1, &p);
        func_800F2D50(n, 8, 0);
        o->state++;
        break;
    case 1:
        if (((TObj *)S32(n, 0x90))->b04 == 2) {
            ((TObj *)S32(n, 0x90))->b04 = 3;
            p = *(Pos6 *)&n->a;
            S32(n, 0x90) = func_8002DCC8(2, 2, &p);
            o->state++;
        }
        break;
    case 2:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            func_8005A8A8(0x3E, 0, 1);
            o->state++;
        }
        break;
    case 3:
        func_800F2D50(n, 0, 0);
        n->step = 0;
        n->state = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A60E4 = 0;
        D_800A603D = 1;
        D_800A603E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
