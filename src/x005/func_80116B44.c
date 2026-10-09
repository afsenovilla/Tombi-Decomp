// FUNC 80116b44 436 X005
// MATCHING 80116b44 436
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009D2B0[];
extern unsigned char D_8009D008;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[];
extern short D_1F8001C6;
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_80026c50(int, int, int);

void func_80116B44(TObj *o)
{
    V6 v;
    TObj *p;
    unsigned char *f;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        if (D_8009D008) {
            o->d90 = FUN_8002dcc8(2, 0x66, &v);
        } else {
            o->d90 = FUN_8002dcc8(2, 0x65, &v);
        }
        D_8009C942[0] = 1;
        D_8009D2B0[0] = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        f = &D_8009D008;
        if (*f == 0) {
            FUN_80026c50(0x41, 1, 1);
            *f = 1;
        }
        D_1F8001C6 = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        if (D_8009C93F[0] == 0) {
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
        }
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        o->animFrame = o->wbc;
        break;
    }
}
