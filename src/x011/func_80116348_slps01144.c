// VERSION SLPS_01144 x11 80116348 1116
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801188B4[];
extern unsigned char D_8009E4E8, D_8009E4EA, D_8009EE58, D_800A7B7C, D_800A7B7D, D_800A7B7E, D_8009EC2A;
extern unsigned short D_800A7B8A;
extern short D_800A7C2A;
extern Fix16 *D_800A7BBC;
extern void func_80020DE4(TObj *);
extern int func_8002E42C(int, int, V6 *);
extern void func_8001C88C(int, int, int);
extern TObj *func_80017B3C(void);

void func_80116348_slps01144(TObj *o)
{
    V6 v;
    TObj *p;
    TObj *r;

    switch (o->state) {
    case 0:
        D_8009E4E8 = 0;
        if ((unsigned)(D_800A7B8A - 64) >= 32) break;
        if (D_800A7BBC->p.whole != 0) break;
        if ((*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C8) == 0) break;
        D_8009E4EA = 1;
        D_8009EE58 = 2;
        D_800A7B7C = 5;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(V6 *)&o->a;
        o->anim = D_801188B4[o->subtype].anims[1];
        func_80020DE4(o);
        o->d90 = func_8002E42C(2, 6, &v);
        o->state++;
        break;
    case 1:
        if (((TObj *)o->d90)->b04 != 2) break;
        ((TObj *)o->d90)->b04 = 3;
        *(short *)0x1F8001C6 = 0;
        D_8009E4EA = 0;
        D_800A7C2A = 0;
        o->animFrame = 1;
        r = func_80017B3C();
        if (r != 0) {
            r->active = 2;
            r->type = 15;
            r->subtype = 51;
            r->b0c = 128;
            r->b0f = 0;
            r->animFrame = 0;
        }
        o->anim = D_801188B4[o->subtype].anims[0];
        func_80020DE4(o);
        o->state = 2;
        break;
    case 2:
        if (D_8009EC2A != 1) break;
        v = *(V6 *)&o->a;
        o->d90 = func_8002E42C(2, 7, &v);
        o->state++;
        break;
    case 3:
        if (((TObj *)o->d90)->b04 != 2) break;
        ((TObj *)o->d90)->b04 = 3;
        D_8009EC2A = 2;
        o->state++;
        break;
    case 4:
        if (D_8009EC2A != 3) break;
        v = *(V6 *)&o->a;
        o->d90 = func_8002E42C(2, 8, &v);
        o->state++;
        break;
    case 5:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        func_8001C88C(0x68, 0, 0);
        o->timer = 360;
        o->state++;
        break;
    case 6:
        if (--o->timer != 0) break;
        o->animFrame = 0;
        v = *(V6 *)&o->a;
        o->d90 = func_8002E42C(2, 9, &v);
        o->state++;
        break;
    case 7:
        if (((TObj *)o->d90)->b04 != 2) break;
        ((TObj *)o->d90)->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = func_8002E42C(2, 10, &v);
        o->state++;
        break;
    case 8:
        if (((TObj *)o->d90)->b04 != 2) break;
        ((TObj *)o->d90)->b04 = 3;
        o->state++;
        break;
    case 9:
        D_8009E4EA = 0;
        D_800A7C2A = 0;
        asm("");
        {
        unsigned short w = o->wbc;
        *(short *)0x1F8001C6 = 0;
        o->animFrame = w;
        }
        asm("");
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
