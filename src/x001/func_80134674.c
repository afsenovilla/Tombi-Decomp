// FUNC 80134674 720 X001
// MATCHING 80134674 720
#include "TOBJ.H"

#define Q ((TObj *)o->d90)

extern void *D_8013F034[];
extern char D_8013F290[];
extern char D_8013F308[];
extern char D_8013F300[];
extern char D_8013F2F8[];
extern char D_8013F298[];
extern char D_8013F2A0[];
extern char D_8013F2A8[];
extern char D_8013F2B0[];
extern char D_8013F410[];
extern char D_8013F424[];
extern char D_8013F438[];
extern char D_8013F44C[];
extern char D_8013F460[];
extern char D_8013F474[];
extern char D_8013F488[];
extern char D_8013F49C[];
extern char D_8013F4B0[];
extern char D_8013F4C4[];
extern char D_8013F4D8[];
extern char D_8013F4EC[];
extern char D_8013F500[];
extern char D_8013F514[];
extern char D_8013F528[];
extern char D_8013F53C[];
extern char D_8013F550[];
extern char D_8013F564[];
extern char D_8013F578[];
extern char D_8013F58C[];
extern char D_8013F5A0[];
extern char D_8013F5B4[];
extern char D_8013F5C8[];
extern char D_8013F5DC[];
extern char D_8013F634[];
extern char D_8013F648[];
extern char D_8013F65C[];
extern char D_8013F670[];
extern char D_8013F684[];
extern char D_8013F698[];
extern char D_8013F6AC[];
extern char D_8013F6C0[];
static void *D_8013F074[48] = {
    D_8013F290, D_8013F308, D_8013F300, D_8013F2F8,
    D_8013F2F8, D_8013F300, D_8013F308, D_8013F290,
    D_8013F298, D_8013F2A0, D_8013F2A8, D_8013F2B0,
    D_8013F2B0, D_8013F2A8, D_8013F2A0, D_8013F298,
    D_8013F410, D_8013F424, D_8013F438, D_8013F44C,
    D_8013F460, D_8013F474, D_8013F488, D_8013F49C,
    D_8013F4B0, D_8013F4C4, D_8013F4D8, D_8013F4EC,
    D_8013F500, D_8013F514, D_8013F528, D_8013F53C,
    D_8013F550, D_8013F564, D_8013F578, D_8013F58C,
    D_8013F5A0, D_8013F5B4, D_8013F5C8, D_8013F5DC,
    D_8013F634, D_8013F648, D_8013F65C, D_8013F670,
    D_8013F684, D_8013F698, D_8013F6AC, D_8013F6C0,
};
extern short D_8007A5F0[], D_8007A3F0[];
extern void FUN_8001e4f0(int);

void func_80134674(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->state++;
        o->active = 2;
        o->velX = 0x10;
        o->b0f -= 0x1e;
        o->velH = -0x400;
        o->velY = 0x40;
        o->velV = -0x400;
        if (o->b0c == 0) {
            for (p = (TObj *)o->d94; p != 0; p = (TObj *)p->d94) {
                p->b04 = 2;
                p->step = 5;
                p->state = 0;
            }
            FUN_8001e4f0(0x54);
            o->d84 = o->d88;
            if ((unsigned int)(o->d88 - 0x40) < 0x80)
                o->animFrame = 1;
            else
                o->animFrame = 0;
            o->wac = ((o->d88 + 8) & 0xff) >> 4;
            o->anim = D_8013F034[o->wac];
        } else {
            if ((o->animFrame = Q->animFrame) == 0)
                o->d84 = (Q->d84 + 0x10) & 0xff;
            else
                o->d84 = (Q->d84 - 0x10) & 0xff;
            o->h->raw = Q->h->raw;
            o->y.raw = Q->y.raw;
            o->d->raw = Q->d->raw;
            o->h->raw -= D_8007A5F0[(unsigned char)o->d84] * 192;
            o->y.raw -= D_8007A3F0[(unsigned char)o->d84] * 192;
            o->wac = ((o->d84 + 8) & 0xff) >> 4;
            o->anim = D_8013F074[o->wac];
        }
        break;
    case 1:
        if (o->velH != 0) {
            if (o->animFrame)
                o->h->raw += o->velH << 8;
            else
                o->h->raw -= o->velH << 8;
            o->velH += o->velX;
        }
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if (o->velV > 0x400)
            o->velV = 0x400;
        break;
    }
}
