// VERSION SLPS_01144 x11 80117260 612
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)



#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009E4EA, D_8009EE58, D_800A7B7C, D_800A7B7D, D_800A7B7E;
extern short D_800A7C2A;
extern short D_1F8001C6_[];
#define D_1F8001C6 D_1F8001C6_[0]
extern int showMessageBoxDirect(int, int, V6 *);
extern void setEventStarted(int, int, int);

void func_80117260_slps01144(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        D_8009E4EA = 1;
        D_8009EE58 = 2;
        D_800A7B7C = 5;
        D_800A7B7D = 0;
        D_800A7B7E = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 0xd, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setEventStarted(0x69, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 2:
        if (--o->timer != 0) break;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 0xe, &v);
        o->state++;
        break;
    case 3: {
        TObj *q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 0xf, &v);
        o->state++;
        break;
    }
    case 4: {
        TObj *q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->state++;
        break;
    }
    case 5:
        D_8009E4EA = 0;
        D_800A7C2A = 0;
        {   /* permuter: wbc read before the scratchpad store, animFrame written after */
            short w;
            do { w = o->wbc; } while (0);
            D_1F8001C6 = 0;
            o->animFrame = w;
        }
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
