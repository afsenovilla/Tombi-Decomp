// FUNC 8012ccf8 1436 X000
// MATCHING 8012ccf8 1436
#include "camera.h"
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"

typedef struct { unsigned char b0, b1; } B8012CCF8;

extern int D_1F800190;
extern int D_1F80018C;
extern unsigned char D_800A4553[];
extern void komaPigSubtype00Step();
extern void komaPigSubtype02Step();
extern void komaPigSubtype03Step();
extern void komaPigSubtype04Step();
extern void komaPigSubtype05Step();
extern void komaPigSubtype06Step();
extern void komaPigSubtype07Step();
extern void komaPigSubtype09Step();
static void (*D_801390F8[11])(TObj *) = {
    komaPigSubtype00Step, komaPigSubtype00Step, komaPigSubtype02Step, komaPigSubtype03Step,
    komaPigSubtype04Step, komaPigSubtype05Step, komaPigSubtype06Step, komaPigSubtype07Step,
    komaPigSubtype00Step, komaPigSubtype09Step, komaPigSubtype03Step,
};
extern int isObjectInView(TObj *);
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void komaPigState0(TObj *);
extern void komaPigState1(TObj *);
extern void komaPigState2(TObj *);
extern void komaPigState3(TObj *);
extern void updateA00L2Type2C(TObj *);
extern void komaPigState5(TObj *);
extern void komaPigState6(TObj *);
extern void komaPigState7(TObj *);
extern void komaPigHit(TObj *);
extern void komaPigGrabbed(TObj *);
extern void komaPigDefeated(TObj *);
extern void spawnPoolType32At(char, int, int, int);
extern TObj *allocObjectLayer2(void);
extern void freeObjectLayer2(TObj *);

void updateKomaPig(TObj *o)
{
    B8012CCF8 *p = (B8012CCF8 *)&o->wb4;
    TObj *n;
    int t;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->box0 = 10;
            o->box1 = 0x14;
            t = o->animFrame;
            o->box2 = 0x10;
            o->box3 = 0x20;
            o->animFrame = t & 1;
            if (t & 1)
                o->wb8 = -0x10;
            else
                o->wb8 = 0x10;
            o->d3c = SPAD.i2d0;
            o->w1e = 1;
            o->b0a = 2;
            o->b0d = 0;
            o->b69 = 0;
            o->b68 = 0;
            *(signed char *)&o->b0f = -9;
            p->b1 = 0;
            o->d8c = 0;
            o->w98 = 3;
            o->anim = 0;
            o->step++;
            break;
        case 1:
            if (isObjectInView(o)) {
                o->step = 0;
                o->b04++;
            }
            break;
        }
        break;
    case 1:
        if (o->step == 1 && ((unsigned char*)&GVARS.b42)[0]) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        if (o->b9e == 0) {
            switch (o->step) {
            case 0:
                if (o->state != 0 && ((unsigned char*)&GVARS.b42)[0]) {
                    ObjCullRegister(o);
                    break;
                }
                if (o->w98 == 0) {
                    o->active = 2;
                    o->b04 = 2;
                    o->step = 2;
                    o->state = 0;
                    break;
                }
                if (o->visible == 0)
                    break;
                D_801390F8[o->subtype](o);
                break;
            case 1:
                if (o->visible == 0)
                    break;
                switch (o->state) {
                case 0:
                    komaPigState0(o);
                    break;
                case 1:
                    komaPigState1(o);
                    break;
                case 2:
                    komaPigState2(o);
                    break;
                case 3:
                    komaPigState3(o);
                    break;
                case 4:
                    updateA00L2Type2C(o);
                    break;
                case 5:
                    komaPigState5(o);
                    break;
                case 6:
                    komaPigState6(o);
                    break;
                case 7:
                    komaPigState7(o);
                    break;
                case 8:
                case 9:
                    break;
                }
                if (o->w98 == 0) {
                    o->active = 2;
                    o->b04 = 2;
                    o->step = 2;
                    o->state = 0;
                }
                break;
            }
        } else if (o->b9f == 0) {
            U16(o, 0xd2) = o->h->p.whole;
            o->b9f = 0xf;
        } else {
            o->h->p.whole = (short)(S16(o, 0xd2) - 2) + (Rand() & 3);
            if (--o->b9f == 0) {
                o->b9e = 0;
                o->h->p.whole = U16(o, 0xd2);
            }
        }
        o->b9d = 0;
        break;
    case 2:
        switch (o->step) {
        case 0:
            if (((unsigned char*)&GVARS.b42)[0]) {
                ObjCullRegister(o);
                break;
            }
            komaPigHit(o);
            ObjCullRegister(o);
            if (o->w98 == 0) {
                o->active = 2;
                o->b04 = 2;
                o->step = 2;
                o->state = 0;
            }
            break;
        case 1:
            komaPigGrabbed(o);
            ObjCullRegister(o);
            break;
        case 2:
            komaPigDefeated(o);
            if (ObjCullRegister(o) == 0)
                o->b04 = 3;
            break;
        case 3:
            ObjCullRegister(o);
            switch (o->state) {
            case 0:
                ((unsigned char*)&GVARS.b3f)[0] = 1;
                ((unsigned char*)&GVARS.b42)[0] = 1;
                D_800A4553[0] = 3;
                CAMERA_SCROLL.velY = 0;
                D_1F80018C = o->h->raw;
                D_1F800190 = o->y.raw;
                spawnPoolType32At(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                spawnPoolType32At(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                n = allocObjectLayer2();
                if (n) {
                    n->active = 1;
                    n->type = 0x2b;
                    n->b6b = o->b6b;
                    n->b1d = o->b1d;
                    n->a.p.whole = o->a.p.whole;
                    n->y.p.whole = o->y.p.whole;
                    n->b.p.whole = o->b.p.whole;
                }
                o->timer = 10;
                o->state++;
            case 1:
                AnimAdvance(o);
                if (--o->timer == -1)
                    o->b04 = 3;
                break;
            }
            break;
        }
        break;
    case 3:
        if (o->subtype == 5)
            (*(short *)o->d94)--;
        freeObjectLayer2(o);
        break;
    }
}
