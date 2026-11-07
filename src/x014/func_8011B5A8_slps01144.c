// VERSION SLPS_01144 x14 8011B5A8 824
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    unsigned short w0, w2, w4, w6, w8, wa, wc, we, w10, w12;
} W;
extern unsigned char D_80122EF0[], D_80122ED0[], D_80122EE0[], D_80122FA8[];
extern unsigned short D_80123038[];
extern void (*D_80122F00[])(TObj *);
extern unsigned short D_8009E50A;
extern void ObjSetFacingToPlayer(TObj *);
extern int Rand(void);
extern int angleFromDelta(short, short);
extern int AnimAdvance(TObj *);

#define WC6(o) (*(unsigned short *)((char *)(o) + 0xc6))
static __inline__ int chk(TObj *o)
{
    if (D_8009E50A != 7) return 0;
    WC6(o) = 0;
    if ((unsigned short)(o->h->p.whole - (*(short*)((char*)&SPAD.i168 + 2)) + 0x40) > 0x80) return 0;
    if ((unsigned short)(o->y.p.whole - (*(short*)((char*)&SPAD.i16c + 2)) + 0x40) > 0x80) return 0;
    if (D_80122FA8[Rand() & 0xf] != 0) WC6(o) = 1; else WC6(o) = 2;
    return 1;
}

void func_8011B5A8_slps01144(TObj *o)
{
    W *w = (W *)&o->wb4;
    unsigned char *t;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        ObjSetFacingToPlayer(o);
        if (w->w12 == 0) {
            if (w->w0 < 1800) t = D_80122EF0;
            else if (w->w0 < 3600) t = D_80122EE0;
            else t = D_80122ED0;
            if (t[Rand() & 0xf] == 0) {
                o->state = 0;
                o->step++;
                break;
            }
        }
        o->d38 = angleFromDelta((*(short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole,
                                (*(short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole) & 0xff;
        asm("");
        o->wb6 = D_8009E50A;
        o->wb8 = ((o->d38 + 8) & 0xff) >> 4;
        o->state++;
        if (w->w12 < 2) {
            if (w->w10 != 0) {
                if ((int)(w->w10 - 1) < (int)w->wc) w->wc = 0;
                w->w2 = D_80123038[w->wc];
                w->wc++;
            } else if (D_80122ED0[Rand() & 0xf] != 0) {
                w->w2 = 7;
            } else {
                o->state = 2;
                o->timer = 40;
                break;
            }
        } else {
            w->w2 = 7;
            w->w12 = 0;
        }
    case 1:
        D_80122F00[w->w2](o);
        if (w->w2 != 7 && o->wac == 18) {
            if (chk(o)) {
                o->step = 1;
                o->state = 0;
                o->substep = 0;
            }
        }
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
#endif
