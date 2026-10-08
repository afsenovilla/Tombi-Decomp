// FUNC 801167f0 412 X005
// MATCHING 801167f0 412
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CDD5;
extern unsigned char D_8009D2B0;
extern unsigned char D_8009C942;
extern unsigned char D_8009C93F;
extern short D_800A60EA;
extern short D_1F8001C6;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern int FUN_8002dcc8(int, int, V6 *);

void func_801167F0(TObj *o)
{
    V6 v;
    TObj *q;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        if (D_8009D2C3 & 0x10) {
            o->d90 = FUN_8002dcc8(2, 0x58, &v);
        } else if (D_8009CDD5 == 0xff) {
            o->d90 = FUN_8002dcc8(2, 0x4e, &v);
        } else {
            o->d90 = FUN_8002dcc8(2, 0x3c, &v);
        }
        D_8009C942 = 1;
        D_8009D2B0 = 2;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->state++;
        break;
    case 1:
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        D_1F8001C6 = 0;
        D_8009C942 = 0;
        D_800A60EA = 0;
        if (D_8009C93F == 0) {
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
        }
        o->animFrame = o->wbc;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
