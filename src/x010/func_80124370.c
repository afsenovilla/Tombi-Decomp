// FUNC 80124370 1680 X010
// MATCHING 80124370 1680
#include "TOBJ.H"
#include "raw7.h"

extern short D_8007A5F0[], D_8007A3F0[];
extern Fix16 *D_800A6078, *D_800A607C;
extern unsigned short D_800A604E;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int func_80123ECC(TObj *, int);
extern void func_8012418C(TObj *);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

static __inline__ void mv(TObj *o)
{
    int i = o->d38;
    int v = o->velH;
    int c = v * D_8007A5F0[i];
    int sn = v * D_8007A3F0[i];

    o->h->raw += (short)(c >> 12) << 8;
    o->y.raw += (short)(sn >> 12) << 8;
}

static __inline__ void turn(TObj *o)
{
    if (o->subtype == 0 && ((D_1F8001F8 + D_1F800198) & 0xf) == 0) {
        int d = o->d30;
        int b = o->d38;
        int t = d - b;
        unsigned char u = t;
        if (u) {
            if (u < 0x81) {
                if (u >= 2) t = 2;
            } else if (u < 0xff) {
                t = 0xfe;
            }
            o->d38 = b + (unsigned char)t;
            o->d38 = U8(o, 0x38);
        }
    }
}

static __inline__ int near(TObj *o)
{
    if ((unsigned short)(D_800A607C->p.whole - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x64) >= 0xc9) return 0;
    return (unsigned short)(D_800A604E - o->y.p.whole + 0x50) < 0xa1;
}

void func_80124370(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->wac = 0;
        o->anim = ANIMS(o)[0];
        AnimLoadDuration(o);
        o->velH = 0x80;
        o->timer = Rand() & 0x7f;
        o->velX = Rand() & 3;
        if (o->subtype == 0)
            o->d30 = Rand() & 0xff;
        else
            o->d30 = Rand() & 0x80;
        break;
    case 1:
        AnimAdvance(o);
        mv(o);
        turn(o);
        if (--o->timer == -1) {
            o->timer = Rand() & 0xbf;
            o->state++;
            if ((unsigned)((o->d38 - 0x40) & 0xff) >= 0x81)
                o->d30 = 0;
            else
                o->d30 = 0x80;
        }
        if (func_80123ECC(o, U8(o, 0x38))) {
            o->wac = 2;
            o->anim = ANIMS(o)[2];
            AnimLoadDuration(o);
            o->state = 3;
        } else if (o->subtype == 0 && near(o)) {
            o->state = 0;
            o->step++;
        }
        if (((D_1F8001F8 + D_1F800198) & 0x1f) == 0) o->velX = Rand() & 3;
        func_8012418C(o);
        break;
    case 2:
        AnimAdvance(o);
        mv(o);
        turn(o);
        if (--o->timer == -1) {
            o->state = 1;
            o->timer = Rand() & 0xbf;
            if (o->subtype == 0)
                o->d30 = Rand() & 0xff;
            else
                o->d30 = Rand() & 0x80;
        }
        if (func_80123ECC(o, U8(o, 0x38))) {
            o->wac = 2;
            o->anim = ANIMS(o)[2];
            AnimLoadDuration(o);
            o->state = 3;
        } else if (o->subtype == 0 && near(o)) {
            o->state = 0;
            o->step++;
        }
        break;
    case 3:
        if (AnimAdvance(o)) {
            o->velH = 0x100;
            o->timer = 0x10;
            o->wac = 0;
            o->state++;
            o->d38 = (o->d38 + 0x80) & 0xff;
            o->anim = ANIMS(o)[0];
            AnimLoadDuration(o);
        }
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state = 1;
            o->timer = Rand() & 0xbf;
        }
        mv(o);
        func_80123ECC(o, U8(o, 0x38));
        break;
    }
}
