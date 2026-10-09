// FUNC 8012cfe0 672 X003
// MATCHING 8012cfe0 672
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009C942[], D_8009C93F[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009D0B8;
extern void *D_801399BC[], *D_801399C0[], *D_801399C4[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void FUN_8005a9a4(int, int);
extern void FUN_80026c50(int, int, int);
extern int FUN_8002dcc8(int, int, V6 *);

void func_8012CFE0(TObj *o)
{
    V6 v;
    TObj *p;

    v = *(V6 *)&o->a;
    switch (o->state) {
    case 0:
        FUN_8001f8e4(o);
        o->wac = 5;
        o->anim = D_801399BC[0];
        AnimLoadDuration(o);
        o->d90 = FUN_8002dcc8(6, 7, &v);
        o->state++;
        break;
    case 1:
    case 5:
    case 9:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->state++;
        }
        break;
    case 2:
        FUN_8005a9a4(0xad, 0);
        o->timer = 0x12c;
        o->state++;
        break;
    case 3:
    case 7:
        if (--o->timer == 0) o->state++;
        break;
    case 4:
        o->d90 = FUN_8002dcc8(6, 8, &v);
        o->state++;
        break;
    case 6:
        o->state++;
        if (D_8009D0B8 == 0) {
            FUN_80026c50(0x14, 1, 1);
            o->timer = 0x50;
            o->w22 = 1;
        } else {
            o->timer = 1;
            o->w22 = 0;
        }
        break;
    case 8:
        o->wac = 6;
        o->anim = D_801399C0[0];
        AnimLoadDuration(o);
        o->state++;
        do { } while (0); /* debt: block boundary (game keeps the args after the state store) */
        o->d90 = FUN_8002dcc8(6, 9, &v);
        break;
    case 10:
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        o->velV = -0x200;
        o->animFrame = 1;
        o->wac = 7;
        o->state++;
        o->anim = D_801399C4[0];
        AnimLoadDuration(o);
    case 11:
        AnimAdvance(o);
        o->a.p.whole -= 4;
        o->velV += 0x10;
        if (o->velV > 0x300) o->velV = 0x300;
        o->y.raw += o->velV << 8;
        if (!o->visible) o->b04 = 3;
        break;
    }
}
