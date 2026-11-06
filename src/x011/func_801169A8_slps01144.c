// VERSION SLPS_01144 x11 801169A8 892
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801188B4[];
extern unsigned char D_8009E4EA, D_8009EE58, D_800A7B7C, D_800A7B7D, D_800A7B7E, D_8009EC2A;
extern short D_800A7C2A;
extern void func_80020DE4(TObj *);
extern int func_8002E42C(int, int, V6 *);
extern void func_80027A5C(int, int, int);
extern void func_8001C988(int, int);
extern TObj *func_80017B3C(void);

void func_801169A8_slps01144(TObj *o)
{
    V6 v;
    TObj *p;
    TObj *q, *r;
    unsigned char *g;

    switch (o->state) {
    case 0:
        v = *(V6 *)&o->a;
        D_8009E4EA = 1;
        D_8009EE58 = 2;
        D_800A7B7C = 5;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->d90 = func_8002E42C(3, 7, &v);
        o->state++;
        break;
    case 1:
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = func_8002E42C(3, 8, &v);
        o->state++;
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        func_80027A5C(0x33, 1, 1);
        func_8001C988(0x68, 0);
        o->timer = 360;
        o->state++;
        break;
    case 3:
        if (--o->timer != 0) break;
        v = *(V6 *)&o->a;
        o->d90 = func_8002E42C(3, 20, &v);
        o->state++;
        break;
    case 4:
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        *(short *)0x1F8001C6 = 0;
        D_8009E4EA = 0;
        D_800A7C2A = 0;
        r = func_80017B3C();
        if (r != 0) {
            r->active = 2;
            r->type = 15;
            r->subtype = 51;
            r->b0c = 128;
            r->b0f = 0;
            r->animFrame = 0;
        }
        o->anim = D_801188B4[o->subtype].anims[1];
        func_80020DE4(o);
        o->state++;
        break;
    case 5:
        g = &D_8009EC2A;
        if (*g != 1) break;
        o->velX = -512;
        o->animFrame = 1;
        o->timer = 60;
        *g = 4;
        o->anim = D_801188B4[o->subtype].anims[0];
        func_80020DE4(o);
        o->state++;
        break;
    case 6:
        o->a.raw += o->velX << 8;
        if (--o->timer != 0) break;
        o->state++;
        break;
    case 7:
        D_800A7C2A = 0;
        asm("");
        o->animFrame = o->wbc;
        asm("");
        D_800A7B7C = 1;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        o->b68 = 0;
        o->b04 = 2;
        break;
    }
}
#endif
