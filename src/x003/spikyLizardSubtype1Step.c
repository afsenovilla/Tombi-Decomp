// FUNC 80126e24 276 X003
// MATCHING 80126e24 276
#include "spad.h"
#include "TOBJ.H"

extern unsigned char D_80135CB0[];
extern void *D_80139578;
extern char MOVE_TABLE_0[];
extern void AnimLoadDuration(TObj *);
extern void applyFrameVelocityY(TObj *);
extern short TileCollideAt(TObj *, short, short);

void spikyLizardSubtype1Step(TObj *o)
{
    unsigned char *p;

    switch (o->state) {
    case 0:
        o->movetab = MOVE_TABLE_0;
        o->wac = 0x1e;
        o->state++;
        o->anim = D_80139578;
        p = &D_80135CB0[o->wac * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        AnimLoadDuration(o);
    case 1:
        applyFrameVelocityY(o);
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe) != 0) {
            o->d8c = (-SPAD.s27e * 4) & 0xff;
            o->state = 0;
            o->step++;
        }
        break;
    }
}
