// FUNC 801268d4 1484 X010
// MATCHING 801268d4 1484
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012F3C4[];
extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_8009CE3D, D_8009CF2F, D_8009CF2E, D_8009CE18;
extern short D_800A60EA;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern void AnimJump(TObj *, int);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a9a4(int, int);
extern void FUN_8005a8a8(int, int, int);

void func_801268D4(TObj *o)
{
    V6 v;
    TObj *e;
    unsigned char n;

    switch (o->state) {
    case 0:
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        if (D_8009CE3D == 0xff) {
            if (!D_8009CF2F) n = 0xf;
            else n = 0x12;
        } else {
            if (!D_8009CF2E) n = 2;
            else n = 8;
        }
        o->state = n;
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 0, &v);
        o->anim = D_8012F3C4[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012F3C4[o->subtype].anims[0];
            AnimJump(o, 0);
            FUN_8005a9a4(0x76, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 4:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(5, 1, &v);
            o->anim = D_8012F3C4[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 5:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012F3C4[o->subtype].anims[0];
            AnimJump(o, 0);
            FUN_8005a8a8(0x7e, 0, 0);
            o->timer = 0x12c;
            o->state++;
        }
        break;
    case 6:
        if (--o->timer <= 0) {
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(5, 2, &v);
            o->anim = D_8012F3C4[o->subtype].anims[2];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 7:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012F3C4[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 8:
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 3, &v);
        o->anim = D_8012F3C4[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 9:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012F3C4[o->subtype].anims[0];
            AnimJump(o, 0);
            D_8009CF2E = 1;
            o->state = 0x1e;
        }
        break;
    case 15:
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 4, &v);
        o->anim = D_8012F3C4[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 16:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012F3C4[o->subtype].anims[0];
            AnimJump(o, 0);
            if (D_8009CE18 == 0) {
                FUN_8005a8a8(0x74, 0, 0);
                o->timer = 0x12c;
            } else {
                o->timer = 1;
            }
            o->state++;
        }
        break;
    case 17:
        if (--o->timer <= 0) {
            o->anim = D_8012F3C4[o->subtype].anims[0];
            AnimJump(o, 0);
            o->state++;
        }
        break;
    case 18:
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(5, 5, &v);
        o->anim = D_8012F3C4[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 19:
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->anim = D_8012F3C4[o->subtype].anims[0];
            AnimJump(o, 0);
            D_8009CF2F = 1;
            o->state = 0x1e;
        }
        break;
    case 30:
        D_800A603C = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
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
