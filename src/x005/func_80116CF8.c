// FUNC 80116cf8 384 X005
// MATCHING 80116cf8 384
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009D2B0;
extern unsigned char D_8009CE17;
extern unsigned char D_8009C93F;
extern short D_800A60EA[];
extern unsigned char D_8009C942[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a8a8(int, int, int);

void func_80116CF8(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(2, 0x62, &v);
        D_8009C942[0] = 1;
        D_8009D2B0 = 2;
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
        if (D_8009CE17 == 0) FUN_8005a8a8(0x73, 0, 0);
        D_1F8001C6 = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        if (D_8009C93F == 0) {
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
