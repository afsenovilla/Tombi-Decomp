// FUNC 8011a9e0 700 X014
// MATCHING 8011a9e0 700
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_80129F94;
extern short NEG_SIN_TABLE[], COS_TABLE[];
extern void FUN_8001fe6c(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern int FUN_800638cc(int);
extern int angleFromDelta(int, int);

static __inline__ void setup(TObj *p)
{
    (*(TObj **)&GVARS.p54) = p;
    p->d3c = ((int*)&SPAD.i2d4)[0];
    p->anim = D_80129F94;
    FUN_8001fe6c(p);
}

void updateBossRoomVortex(TObj *o)
{
    int t = o->b04;
    unsigned char s;
    short dx, dy;
    int d, a;

    switch (t) {
    case 0:
        o->active = 2;
        o->b0a = 2;
        o->w1e = 0x10;
        o->b0d = 0x80;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->timer = 0x14;
        o->b04++;
        setup(o);
        break;
    case 1:
        ObjCullRegister(o);
        o->d8c = (o->d8c + 2) & 0xff;
        s = o->step;
        switch (s) {
        case 0:
            if (--o->timer == -1) o->step++;
            break;
        case 1:
            if (GVARS.b3a == 1) o->step = s + 1;
            break;
        case 2:
            if (GVARS.b3f == 0) {
                dx = (*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole;
                dy = (*(unsigned short*)&PLAYER.obj.y.p.whole) - o->y.p.whole;
                d = FUN_800638cc(dx * dx + dy * dy);
                if (d >= 11) {
                    if (d > 0xa0) d = 0xa0;
                    d = 0xaa - d;
                    a = angleFromDelta(dx, dy) & 0xff;
                    o->d38 = a;
                    d <<= 2;
                    ((short*)&GVARS.s46)[0] = (d * NEG_SIN_TABLE[(a + 0x48) & 0xff]) >> 12;
                    ((short*)&GVARS.s44)[0] = (d * COS_TABLE[(o->d38 + 0x48) & 0xff]) >> 12;
                    break;
                }
            }
            ((short*)&GVARS.s44)[0] = 0;
            ((short*)&GVARS.s46)[0] = 0;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
