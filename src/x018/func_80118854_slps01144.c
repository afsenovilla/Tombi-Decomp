// VERSION SLPS_01144 x18 80118854 2024
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT D_80119560[];
extern TObj PLAYER;
extern unsigned char D_8009EE58, D_8009E4E6, D_8009E4E7, D_8009E4E8, D_8009E4E9, D_8009E4EA;
extern unsigned char D_800A7B7C, D_800A7B7D, D_800A7B7E;
extern unsigned char D_8009EB98[];
extern short D_800A7C2A;
extern short D_1F8001C6;
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void addItemToInventory(int, int, int);
extern void removeItemFromInventory(int, int);
extern void func_8001C988(int, int);

static __inline__ void setanim(TObj *o, short n)
{
    if (((TX *)o)->wd2 != n) {
        o->anim = D_80119560[o->subtype].anims[n];
        AnimJump(o, 0);
        ((TX *)o)->wd2 = n;
    }
}

void func_80118854_slps01144(TObj *o)
{
    V6 v;
    TObj *q;
    short i;
    int k;

    switch (o->state) {
    case 0:
        v = *(V6 *)&o->a;
        k = 1;
        asm volatile("" : "=r"(k) : "0"(k));
        D_8009EE58 = 2;
        D_8009E4EA = k;
        D_8009E4E7 = k;
        D_8009E4E6 = k;
        D_800A7B7C = 5;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        asm volatile("" ::: "memory");
        o->wbc = o->animFrame;
        o->animFrame = PLAYER.h->p.whole < o->h->p.whole;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        switch (D_8009E4E9) {
        case 8: D_8009EB98[0] = k; break;
        case 102: D_8009EB98[1] = k; break;
        case 103: D_8009EB98[2] = k; break;
        case 104: D_8009EB98[3] = k; break;
        case 105: D_8009EB98[4] = k; break;
        case 106: D_8009EB98[5] = k; break;
        }
        removeItemFromInventory(D_8009E4E9, 1);
        for (i = 0; i < 6; i++);
        for (i = 0; i < 6; i++) {
            if (D_8009EB98[i] == 0) break;
        }
        if (i == 6) {
            o->state = 96;
            break;
        }
        o->d90 = showMessageBoxDirect(3, 15, &v);
        o->state = 97;
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->state = 99;
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 16, &v);
        ((TX *)o)->wd2 = 1;
        o->state = 9;
        break;
    case 3:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 17, &v);
        ((TX *)o)->wd2 = 2;
        o->state = 9;
        break;
    case 4:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 18, &v);
        ((TX *)o)->wd2 = 3;
        o->state = 9;
        break;
    case 5:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 19, &v);
        ((TX *)o)->wd2 = 4;
        o->state = 9;
        break;
    case 6:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 20, &v);
        ((TX *)o)->wd2 = 5;
        o->state = 9;
        break;
    case 7:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 21, &v);
        ((TX *)o)->wd2 = 6;
        o->state = 9;
        break;
    case 8:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 22, &v);
        o->animFrame = PLAYER.h->p.whole < o->h->p.whole;
        setanim(o, 4);
        o->state = 1;
        break;
    case 9:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 22, &v);
        o->state = 1;
        break;
    case 96:
        func_8001C988(120, 0);
        addItemToInventory(115, 1, 1);
        o->timer = 200;
        o->state = 98;
        break;
    case 97:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        for (i = 0; i < 6; i++) {
            if (D_8009EB98[i] == 0) break;
        }
        if (i == 6) break;
        v = *(V6 *)&o->a;
        setanim(o, 4);
        o->timer = 0;
        o->state = i + 2;
        break;
    case 98:
        if (--o->timer > 0) break;
        o->state = 99;
        break;
    case 99:
        setanim(o, 0);
        D_8009E4E8 = 0;
        D_8009E4E6 = 0;
        D_8009E4E7 = 0;
        D_8009E4EA = 0;
        D_800A7C2A = 0;
        asm volatile("" ::: "memory");
        { unsigned short t = o->wbc;
        D_1F8001C6 = 0;
        o->animFrame = t; }
        asm volatile("" ::: "memory");
        D_800A7B7C = 1;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
#endif
