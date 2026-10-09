// FUNC 8012962c 492 X003
// MATCHING 8012962c 492
#include "TOBJ.H"
typedef struct { char c[12]; } V12;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80135D84[];
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned short D_800A6066;
extern Fix16 *D_800A6078[];
extern short D_800A60EA;
extern void FUN_8001fe94(TObj *, int);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V12 *);

void func_8012962C(TObj *o)
{
    V12 b;
    TObj *p;
    int t;

    switch (o->state) {
    case 0:
        if (o->b68) {
            D_8009C93F = 1;
            D_8009C942 = 1;
            b = *(V12 *)&o->a;
            o->d90 = FUN_8002dcc8(2, 0xf, &b);
            o->anim = D_80135D84[o->subtype].anims[9];
            FUN_8001fe94(o, 0);
            o->animFrame = o->b68 & 1;
            t = o->h->p.whole < *(short *)((char *)D_800A6078[0] + 2);
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_800A6066 = t;
            o->state++;
        }
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->animFrame = 1;
            o->anim = D_80135D84[o->subtype].anims[0];
            FUN_8001fe94(o, 0);
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009C93E = 0;
            D_800A60EA = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->b68 = 0;
            o->state = 0;
        }
        break;
    }
    if (!o->visible) o->b04 = 3;
}
