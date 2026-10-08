// FUNC 80118028 644 X002
// MATCHING 80118028 644
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct {
    TObj t;
    char pad[0x12];
    unsigned short wd2;
} TO2;
extern AT D_8011C778[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int Rand(void);
extern int FUN_8002dcc8(int, int, V6 *);

void func_80118028(TO2 *o)
{
    V6 v;
    TObj *p;

    switch (o->t.state) {
    case 0:
        if (o->t.b68 == 0) break;
        v = *(V6 *)&o->t.a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        if (o->wd2 != 0) {
            o->t.anim = D_8011C778[o->t.subtype].anims[0];
            AnimJump(&o->t, 0);
            o->wd2 = 0;
        }
        o->t.wbc = o->t.animFrame;
        o->t.animFrame = o->t.b68 & 1;
        switch (Rand() & 3) {
        case 0:
        case 1:
            o->t.d90 = FUN_8002dcc8(3, 0, &v);
            break;
        case 2:
            o->t.d90 = FUN_8002dcc8(3, 1, &v);
            break;
        case 3:
            o->t.d90 = FUN_8002dcc8(3, 2, &v);
            break;
        }
        o->t.state++;
        break;
    case 1:
        AnimAdvance(&o->t);
        p = (TObj *)o->t.d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        if (o->wd2 != 0) {
            o->t.anim = D_8011C778[o->t.subtype].anims[0];
            AnimJump(&o->t, 0);
            o->wd2 = 0;
        }
        D_800A60EA[0] = 0;
        o->t.timer = 4;
        o->t.state = 9;
        break;
    case 9:
        D_8009C942[0] = 0;
        D_8009C93F[0] = 0;
        D_800A60EA[0] = 0;
        o->t.animFrame = o->t.wbc;
        D_1F8001C6 = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->t.b68 = 0;
        o->t.step = 0;
        o->t.state = 0;
        break;
    }
}
