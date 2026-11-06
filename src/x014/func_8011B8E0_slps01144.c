// VERSION SLPS_01144 x14 8011B8E0 464
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned short D_8009E50A;
extern unsigned char D_80122ED0[];
extern unsigned char D_80122EE0[];
extern unsigned char D_80122EF0[];
extern void (*D_80122F00[])(TObj *);
extern void func_8011B5A8(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern int angleFromDelta(int, int);

void func_8011B8E0_slps01144(TObj *o)
{
    unsigned char *t;
    int x;

    if (D_8009E50A == 7) {
        func_8011B5A8(o);
    } else switch (o->state) {
    case 0:
        ObjSetFacingToPlayer(o);
        if ((unsigned short)o->wb4 < 0x708) {
            t = D_80122EF0;
        } else {
            t = D_80122ED0;
            if ((unsigned short)o->wb4 < 0xe10) {
                t = D_80122EE0;
            }
        }
        if (t[Rand() & 0xf] == 0) {
            o->state = 0;
            o->step++;
            break;
        }
        o->d38 = angleFromDelta((short)((*(unsigned short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole), (short)((*(unsigned short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole)) & 0xff;
        asm("" ::: "memory");
        o->wb6 = D_8009E50A;
        x = ((o->d38 + 8) & 0xff) >> 4;
        o->wb8 = x;
        o->state++;
    case 1:
        D_80122F00[D_8009E50A](o);
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
