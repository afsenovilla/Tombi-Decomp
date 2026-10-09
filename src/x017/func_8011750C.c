// FUNC 8011750c 856 X017
// MATCHING 8011750c 856
#include "TOBJ.H"
typedef struct { char c[12]; } V12;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119990[];
extern unsigned char D_8009C942[], D_8009C93F[];
extern unsigned char D_8009D2B0[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009CE1A[], D_8009D13F;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern int FUN_8002dcc8(int, int, V12 *);
extern void FUN_80026c50(int, int, int);

void func_8011750C(TObj *o)
{
    V12 b;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68) {
            b = *(V12 *)&o->a;
            D_8009C942[0] = 1;
            D_8009C93F[0] = 1;
            D_8009D2B0[0] = 2;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->anim = D_80119990[o->subtype].anims[2];
            FUN_8001fe94(o, 0);
            o->wbc = o->animFrame;
            o->animFrame = o->b68 & 1;
            if (D_8009CE1A[0] == 1) {
                o->d90 = FUN_8002dcc8(2, 0x26, &b);
                o->state = 3;
            } else {
                o->d90 = FUN_8002dcc8(2, 0x23, &b);
                o->state++;
            }
        }
        break;
    case 1:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_1F8001C6 = 0;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A60EA[0] = 0;
            o->timer = 4;
            o->anim = D_80119990[o->subtype].anims[0];
            FUN_8001fe94(o, 0);
            o->state = 2;
        }
        break;
    case 2:
        FUN_8001fec0(o);
        if (--o->timer) o->state = 9;
        break;
    case 3:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_1F8001C6 = 0;
            D_8009C942[0] = 0;
            D_8009C93F[0] = 0;
            D_800A60EA[0] = 0;
            o->timer = 4;
            o->anim = D_80119990[o->subtype].anims[0];
            FUN_8001fe94(o, 0);
            if (!D_8009D13F) {
                FUN_80026c50(0x9b, 1, 1);
                o->timer = 200;
            }
            o->state = 2;
        }
        break;
    case 9:
        D_8009C942[0] = 0;
        D_8009C93F[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        o->anim = D_80119990[o->subtype].anims[0];
        FUN_8001fe94(o, 0);
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
