// FUNC 80129be0 564 X004
// MATCHING 80129be0 564
/* debt: volatile d90 store + volatile subtype read keep sw d90 before lbu subtype in case 1 (do {} while (0) after the store gives score 4). */
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801311B8[];
extern unsigned char D_8009C942[], D_8009C93F[], D_8009C93E[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[];
extern unsigned char D_8009CDDC;
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a8a8(int, int, int);

void func_80129BE0(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->animFrame = o->b68 & 1;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        v = *(V6 *)&o->a;
        *(volatile int *)&o->d90 = FUN_8002dcc8(2, 1, &v);
        o->anim = D_801311B8[*(volatile unsigned char *)&o->subtype].anims[3];
        AnimJump(o, 0);
        o->state++;
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_801311B8[o->subtype].anims[0];
        AnimJump(o, 0);
        o->state = 15;
        if (D_8009CDDC == 0) {
            o->state = 3;
            o->timer = 200;
            FUN_8005a8a8(0x38, 0, 0);
        }
        break;
    case 3:
        if (--o->timer == 0) o->state = 15;
        break;
    case 15:
        D_800A603C[0] = 1;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        D_800A60EA[0] = 0;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
