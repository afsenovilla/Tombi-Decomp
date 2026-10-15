// FUNC 8012efc4 1156 X001
// MATCHING 8012efc4 1156
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

typedef struct { TObj o; char pc0[0x12]; unsigned short d2; } TObjX;
typedef void (*Fn)(TObj *);

extern int DAT_1f8002d0[];
extern void func_8012EAE8();
static Fn D_8013C8FC[11] = {
    (Fn)func_8012EAE8, (Fn)func_8012EAE8, (Fn)func_8012EAE8, (Fn)func_8012EAE8,
    (Fn)func_8012EAE8, (Fn)func_8012EAE8, (Fn)func_8012EAE8, (Fn)func_8012EAE8,
    (Fn)func_8012EAE8, (Fn)func_8012EAE8, (Fn)func_8012EAE8,
};
extern short D_800A4582;
extern int FUN_800202b4(TObj *);
extern unsigned int FUN_8001f9e0(void);
extern void despawnObjectOffscreenMargin(TObj *, int);
extern void func_8012C8EC(TObj *);
extern void func_8012C0EC(TObj *);
extern void func_8012BD6C(TObj *);
extern void komaPigHit(TObj *);
extern void komaPigGrabbed(TObj *);
extern void komaPigDefeated(TObj *);
extern void komaPigGrabbedB(TObj *);
extern void freeObjectLayer2(TObj *);

void updateKomaPig(TObjX *e)
{
    TObj *o = &e->o;
    unsigned char *q = (unsigned char *)&o->wb4;
    int k;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            if (o->animFrame != 0)
                o->wb8 = -0x10;
            else
                o->wb8 = 0x10;
            o->box0 = 0xa;
            o->box1 = 0x14;
            o->box2 = 0x10;
            o->box3 = 0x20;
            o->d3c = DAT_1f8002d0[0];
            o->w1e = 1;
            o->b0a = 2;
            o->b0d = 0;
            o->b69 = 0;
            o->b68 = 0;
            *(signed char *)&o->b0f = -9;
            q[1] = 0;
            o->d8c = 0;
            o->w98 = 3;
            o->anim = 0;
            o->step++;
            break;
        case 1:
            o->b04++;
            o->step = 2;
            break;
        }
        break;
    case 1:
        if (GVARS.b42 != 0) {
            if (o->visible != 0)
                FUN_800202b4(o);
            break;
        }
        if (o->b9e == 0) {
            switch (o->step) {
            case 0:
            case 1:
            case 2:
                D_8013C8FC[o->subtype](o);
                if (o->b0c != 0)
                    break;
                goto chk;
            case 3:
                FUN_800202b4(o);
                switch (o->state) {
                case 0:
                    func_8012C8EC(o);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                    func_8012C0EC(o);
                    break;
                case 9:
                    func_8012BD6C(o);
                    break;
                }
                if (o->b0c == 0) {
                    if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0)
                        despawnObjectOffscreenMargin(o, 0x50);
                    break;
                }
            chk:
                if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0 && D_800A4582 + 0xa0 < o->y.p.whole)
                    o->b04 = 3;
                break;
            }
        } else {
            FUN_800202b4(o);
            if (o->b9f == 0) {
                e->d2 = o->h->p.whole;
                o->b9f = 0xf;
            } else {
                k = 2;
                o->h->p.whole = e->d2 - k + (FUN_8001f9e0() & 3);
                if (--o->b9f == 0) {
                    o->b9e = 0;
                    o->h->p.whole = e->d2;
                }
            }
        }
        o->b9d = 0;
        break;
    case 2:
        switch (o->step) {
        case 0:
            if (GVARS.b42 != 0) {
                FUN_800202b4(o);
                break;
            }
            komaPigHit(o);
            FUN_800202b4(o);
            if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0 && D_800A4582 + 0xa0 < o->y.p.whole)
                o->b04 = 3;
            break;
        case 1:
            komaPigGrabbed(o);
            FUN_800202b4(o);
            break;
        case 2:
            komaPigDefeated(o);
            if (FUN_800202b4(o) == 0)
                o->b04 = 3;
            break;
        case 3:
            komaPigGrabbedB(o);
            FUN_800202b4(o);
            break;
        }
        break;
    case 3:
        if (o->subtype == 8 && o->b0c == 0)
            (*(short *)o->d94)--;
        freeObjectLayer2(o);
        break;
    }
}
