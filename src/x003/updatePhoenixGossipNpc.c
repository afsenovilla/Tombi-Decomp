// FUNC 801307b0 480 X003
// MATCHING 801307b0 480
#include "spad.h"
#include "gcd.h"
#include "gv2.h"
#include "TOBJ.H"

extern char D_8013A558[];
extern char D_8013A560[];
extern char D_8013A594[];
extern char D_8013A5C8[];
static void *D_8013A368[4] = {
    D_8013A558, D_8013A560, D_8013A594, D_8013A5C8,
};
extern void *D_8013A3A8[];
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);
extern void phoenixGossipAskTalk(TObj *);
extern void phoenixGossipLavaHint(TObj *);
extern void phoenixGossipAskTalkB(TObj *);
extern void phoenixGossipLavaHintB(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void updatePhoenixGossipNpc(TObj *o)
{
    unsigned char t = o->b04;
    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->active = 3;
        *(signed char *)&o->b0f = -4;
        if (!(GV2.bc3 & 0x10)) {
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0xc;
            o->box3 = 0x1c;
            o->w1e = 1;
            o->b0d = 0;
            o->d3c = ((int*)&SPAD.i2d0)[0];
            o->wac = 0;
            setAnim(o, D_8013A368[0]);
        } else {
            o->box0 = 10;
            o->box1 = 0x14;
            o->box2 = 0x18;
            o->box3 = 0x2e;
            o->w1e = 10;
            o->b0d = 1;
            o->w08 = FUN_8005e420(0xd0, 0x1e2);
            o->d3c = ((int*)&SPAD.i2f8)[0];
            o->wac = 0;
            setAnim(o, D_8013A3A8[0]);
        }
        break;
    case 1:
        AnimAdvance(o);
        ObjCullRegister(o);
        if (!(GV2.bc3 & 0x10)) {
            if (GCD.bcdd9 == 0) phoenixGossipAskTalk(o);
            else phoenixGossipLavaHint(o);
        } else {
            if (GCD.bcdd9 == 0) phoenixGossipAskTalkB(o);
            else phoenixGossipLavaHintB(o);
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
