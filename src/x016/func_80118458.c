// FUNC 80118458 972 X016
// MATCHING 80118458 972
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT D_80118D34[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA;
extern unsigned char D_8009C942, D_8009C940;
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void addItemToInventory(int, int, int);
extern void removeItemFromInventory(int, int);

void func_80118458(TObj *o)
{
    V6 v;
    TObj *q;
    short n;

    switch (o->state) {
    case 0:
        v = *(V6 *)&o->a;
        D_8009C942 = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->d90 = FUN_8002dcc8(2, 0x11, &v);
        n = o->animFrame + 2;
        if (n != ((TX *)o)->wd2) {
            o->anim = D_80118D34[o->subtype].anims[n];
            AnimJump(o, 0);
            ((TX *)o)->wd2 = n;
        }
        o->state = 1;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        break;
    case 1:
        AnimAdvance(o);
        { TObj *q1 = (TObj *)o->d90;
        if (q1->b04 != 2) break;
        q1->b04 = 3; }
        o->animFrame ^= 1;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(2, 0x12, &v);
        o->anim = D_80118D34[o->subtype].anims[5];
        AnimJump(o, 0);
        o->timer = 0x78;
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        o->state++;
        break;
    case 3:
        AnimAdvance(o);
        { TObj *q3 = (TObj *)o->d90;
        if (q3->b04 != 2) break;
        q3->b04 = 3; }
        o->animFrame = o->b68 & 1;
        v = *(V6 *)&o->a;
        o->anim = D_80118D34[o->subtype].anims[2];
        AnimJump(o, 0);
        o->d90 = FUN_8002dcc8(2, 0x13, &v);
        o->state++;
        break;
    case 4:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        addItemToInventory(0x8b, 1, 1);
        o->state = 9;
        break;
    case 9:
        D_1F8001C6 = 0;
        D_8009C942 = 0;
        D_8009C940 = 0;
        D_800A60EA = 0;
        o->anim = D_80118D34[o->subtype].anims[0];
        AnimJump(o, 0);
        removeItemFromInventory(3, 1);
        o->animFrame = o->wbc;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    case 10:
        if (--o->timer > 0) break;
        o->state = 9;
        break;
    }
}
