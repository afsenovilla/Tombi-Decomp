// FUNC 80045600 132 MAIN0
// MATCHING 80045600 132
// VERSION SLPS_01144 slps01144 800509D8 132
// VERSION SLPM_80165 slpm80165 8004B708 132
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 80045600 (tryAttachObjectOnOverlap.c): 228 -> 212.
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj o; char p[0xd4 - 0xc0]; TObj *e4; } TObjX;
extern short playerPushOutOfObjectBox(TObjX *o, TObj *p);

void tryAttachObjectOnOverlap(TObjX *o, TObj *p)
{
    short r = playerPushOutOfObjectBox(o, p);
    if (r != 0) {
        if (*(unsigned char *)&o->o.wac == 1 && r == 1) {
            p->active = 2;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->b69 = 0;
            o->e4 = p;
            *(unsigned char *)&o->o.wac = 2;
        }
        SPAD.s19e = 0;
    }
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80045600 (tryAttachObjectOnOverlap.c): attached object at +0xd0 (PAL +0xe4).
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj o; char p[0xd0 - 0xc0]; TObj *e4; } TObjX;
extern short playerPushOutOfObjectBox(TObjX *o, TObj *p);

void tryAttachObjectOnOverlap(TObjX *o, TObj *p)
{
    short r = playerPushOutOfObjectBox(o, p);
    if (r != 0) {
        if (*(unsigned char *)&o->o.wac == 1 && r == 1) {
            p->active = 2;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->b69 = 0;
            o->e4 = p;
            *(unsigned char *)&o->o.wac = 2;
        }
        SPAD.s19e = 0;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj o; char p[0xe4 - 0xc0]; TObj *e4; } TObjX;
extern short playerPushOutOfObjectBox(TObjX *o, TObj *p);

void tryAttachObjectOnOverlap(TObjX *o, TObj *p)
{
    short r = playerPushOutOfObjectBox(o, p);
    if (r != 0) {
        if (*(unsigned char *)&o->o.wac == 1 && r == 1) {
            p->active = 2;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            p->b69 = 0;
            o->e4 = p;
            *(unsigned char *)&o->o.wac = 2;
        }
        SPAD.s19e = 0;
    }
}
#endif
