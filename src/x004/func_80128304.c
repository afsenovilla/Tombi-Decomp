// FUNC 80128304 872 X004
// MATCHING 80128304 872
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8013117C[];
extern unsigned char D_8009C940, D_8009C93F, D_8009C942, D_8009C93E;
extern unsigned char D_8009D138;
extern short D_800A60EA;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern void AnimJump(TObj *, int);
extern int FUN_8002dcc8(int, int, V6 *);
extern void removeItemFromInventory(int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8005a8a8(int, int, int);

void func_80128304(TObj *o)
{
    V6 v;
    TObj *e;

    switch (o->state) {
    case 0:
        D_8009C940 = 0;
        removeItemFromInventory(0x94, D_8009D138);
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        o->state++;
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(4, 1, &v);
        o->anim = D_8013117C[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            FUN_8005a9a4(0x70, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 4:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(4, 2, &v);
            o->anim = D_8013117C[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 5:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            FUN_8005a8a8(0x99, 0, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 6:
        if (--o->timer <= 0) o->state++;
        break;
    case 7:
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(4, 3, &v);
        o->anim = D_8013117C[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 8:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8013117C[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state = 0xf;
        }
        break;
    case 15:
        D_800A603C = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        D_800A60EA = 0;
        D_800A603D = 0;
        D_800A603E = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
