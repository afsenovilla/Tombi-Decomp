// VERSION SLPS_01144 x10 801249FC 1324
// FLAGS -O2 -G0 -fno-strength-reduce
// JP-only: PAL tunnelDiggerTalk family is a different handler; this one is the item-giving NPC talk (16 states).
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
typedef struct { void **p; int pad[2]; } E12;
extern unsigned char D_800A7B7C_[];
#define D_800A7B7C D_800A7B7C_[0]
extern unsigned char D_800A7B7D_[];
#define D_800A7B7D D_800A7B7D_[0]
extern unsigned char D_800A7B7E_[];
#define D_800A7B7E D_800A7B7E_[0]
extern unsigned char D_8009E4E7_[];
#define D_8009E4E7 D_8009E4E7_[0]
extern unsigned char D_8009E4EA_[];
#define D_8009E4EA D_8009E4EA_[0]
extern unsigned char D_8009E4E6_[];
#define D_8009E4E6 D_8009E4E6_[0]
extern short D_800A7C2A_[];
#define D_800A7C2A D_800A7C2A_[0]
extern E12 D_8012D968[];
extern TObj *showMessageBoxDirect(int, int, V *);
extern void AnimJump(TObj *, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void removeItemFromInventory(int, int);
extern void addItemToInventory(int, int, int);

#define START_TALK() \
    D_800A7B7C = 5; \
    D_800A7B7D = 0; \
    D_800A7B7E = 0; \
    D_8009E4E7 = 1; \
    D_8009E4EA = 1

void func_801249FC_slps01144(TObj *o)
{
    V v;
    TObj *p3, *p8, *p10, *p15;

    switch (o->state) {
    case 0:
        START_TALK();
        o->state++;
        break;
    case 2:
        v = *(V *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 10, &v);
        o->anim = D_8012D968[o->subtype].p[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        p3 = (TObj *)o->d90;
        if (p3->b04 != 2)
            break;
        p3->b04 = 3;
        setEventStarted(0x7f, 0, 0);
        o->timer = 300;
        o->state++;
        break;
    case 4:
        if (--o->timer > 0)
            break;
        START_TALK();
        v = *(V *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 11, &v);
        o->anim = D_8012D968[o->subtype].p[2];
        AnimJump(o, 0);
        o->state = 15;
        break;
    case 5:
        START_TALK();
        o->state++;
        break;
    case 7:
        v = *(V *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 12, &v);
        o->anim = D_8012D968[o->subtype].p[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 8:
        p8 = (TObj *)o->d90;
        if (p8->b04 != 2)
            break;
        p8->b04 = 3;
        removeItemFromInventory(89, 1);
        removeItemFromInventory(93, 1);
        removeItemFromInventory(94, 1);
        removeItemFromInventory(95, 1);
        removeItemFromInventory(115, 1);
        addItemToInventory(96, 1, 1);
        setEventComplete(0x7f, 1);
        o->timer = 300;
        o->state++;
        break;
    case 9:
        if (--o->timer > 0)
            break;
        v = *(V *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 13, &v);
        o->anim = D_8012D968[o->subtype].p[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 10:
        p10 = (TObj *)o->d90;
        if (p10->b04 != 2)
            break;
        p10->b04 = 3;
        goto c13;
    case 11:
        START_TALK();
        o->state++;
        break;
    case 1:
    case 6:
    case 12:
        D_8009E4E6 = 1;
        o->state++;
        break;
    case 13:
    c13:
        v = *(V *)&o->a;
        o->d90 = (int)showMessageBoxDirect(5, 14, &v);
        o->anim = D_8012D968[o->subtype].p[2];
        AnimJump(o, 0);
        o->state = 15;
        break;
    case 15:
        p15 = (TObj *)o->d90;
        if (p15->b04 != 2)
            break;
        p15->b04 = 3;
        o->anim = D_8012D968[o->subtype].p[0];
        AnimJump(o, 0);
        D_800A7B7C = 1;
        D_8009E4E7 = 0;
        D_8009E4EA = 0;
        D_8009E4E6 = 0;
        D_800A7C2A = 0;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        *(short *)&o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
#endif
