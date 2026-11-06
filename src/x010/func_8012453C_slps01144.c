// VERSION SLPS_01144 x10 8012453C 1216
// FLAGS -O2 -G0 -fno-strength-reduce
// JP-only rewrite from retail (npcSetAnim is inlined: D_8012D968[subtype].anims[n]).
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

typedef struct { char c[12]; } B12;
typedef struct { void **anims; int a; int b; } AT;

extern unsigned char D_8009EC8F[];
extern AT D_8012D968[];
extern struct { unsigned char v; } D_800A7B7C;
extern struct { unsigned char v; } D_800A7B7D;
extern struct { unsigned char v; } D_800A7B7E;
extern struct { short v; } D_800A7C2A;
extern struct { unsigned char v; } D_8009E4E6;
extern struct { unsigned char v; } D_8009E4E7;
extern struct { unsigned char v; } D_8009E4EA;

extern TObj *showMessageBoxDirect(int, int, void *);
extern void AnimJump(TObj *, short);
extern void func_8001C988(int, int);
extern void setEventStarted(int, int, int);
extern void addItemToInventory(int, int, int);
extern void removeItemFromInventory(int, int);

#define SPAWN(k) \
    { \
        v = *(B12 *)&o->a; \
        o->d90 = (int)showMessageBoxDirect(5, k, &v); \
    }
#define ANIM(n) \
    { \
        o->anim = D_8012D968[o->subtype].anims[n]; \
        AnimJump(o, 0); \
    }

void func_8012453C_slps01144(TObj *o)
{
    TObj *q;
    B12 v;
    short i;

    switch (o->state) {
    case 0:
        D_800A7B7C.v = 5;
        D_800A7B7D.v = 0;
        D_800A7B7E.v = 0;
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        o->state++;
        break;
    case 1:
        D_8009E4E6.v = 1;
        func_8001C988(126, 0);
        o->timer = 300;
        o->state++;
        break;
    case 2:
        if (--o->timer > 0) break;
        SPAWN(6);
        ANIM(2);
        for (i = 0; i < 10; i++)
            removeItemFromInventory(i + 67, D_8009EC8F[i]);
        o->state++;
        break;
    case 3:
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        addItemToInventory(44, 1, 1);
        SPAWN(7);
        o->state++;
        break;
    case 4: {
        TObj *q2 = (TObj *)o->d90;
        if (q2->b04 != 2) break;
        q2->b04 = 3;
        ANIM(0);
        o->timer = 60;
        o->state++;
        }
        break;
    case 5:
        if (--o->timer > 0) break;
        SPAWN(8);
        ANIM(2);
        o->state++;
        break;
    case 6:
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        addItemToInventory(149, 1, 1);
        ANIM(0);
        setEventStarted(156, 0, 0);
        o->timer = 300;
        o->state++;
        break;
    case 7:
        if (--o->timer > 0) break;
        o->state++;
        break;
    case 8:
        D_800A7B7C.v = 5;
        D_800A7B7D.v = 0;
        D_800A7B7E.v = 0;
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        SPAWN(9);
        ANIM(2);
        o->state++;
        break;
    case 9: {
        TObj *q2 = (TObj *)o->d90;
        if (q2->b04 != 2) break;
        q2->b04 = 3;
        ANIM(0);
        D_800A7B7C.v = 1;
        D_8009E4E7.v = 0;
        D_8009E4EA.v = 0;
        D_8009E4E6.v = 0;
        D_800A7C2A.v = 0;
        D_800A7B7D.v = 0;
        D_800A7B7E.v = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        }
        break;
    }
}
#endif
