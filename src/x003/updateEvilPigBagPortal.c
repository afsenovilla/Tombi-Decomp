// FUNC 8011d84c 1016 X003
// MATCHING 8011d84c 1016
#include "spad.h"
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
static char D_80135AA8[64] = {
    0, 0, -0x19, 0x1c, -0x18, 0x18, -0x16, 0x18, -0x14, 0x14, -0x12, 0x10, -0x10, 0x10, -0xe, 0xc,
    0x13, 0xd, 0x15, 9, 0x17, 9, 0x19, 5, 0x1b, 5, 0x1d, 1, 0x3f, 1, -1, 0x7f,
    0, 0, -0x48, 0x4d, 0x7f, 0x6f, 0x1d, 0x67, -0x24, 0x62, 0x7b, 0x5a, 0x39, 0x56, -0x28, 0x4d,
    -0x69, 0x49, 0x5f, 0x6b, -0x23, 0x5a, -0x47, 0x45, 0x36, 0x41, -8, 0x45, 0x5a, 0x4e, -0x64, 0x56,
};
static short D_80135AE8[12] = {
    0, 0, 0, 0, 0, 1, 2, 0,
    0x100, 0x200, 1, 2,
};
extern void FUN_8005f3c0(short *, char *);
extern void FUN_80018cf0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);
extern void evilPigBagPortalOpen(TObj *);
extern void evilPigBagPortalEnter(TObj *);

static __inline__ int onscreen(TObj *o)
{
    if (GVARS.b3a == 0) return 0;
    if ((unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i174 + 2))) >= 0x12d) return 0;
    return (unsigned short)(SPAD.s186 - (o->y.p.whole + 0x10)) < 0xc9;
}

void updateEvilPigBagPortal(TObj *o)
{
    short buf[4];
    TObj *p;
    int dx, dy;

    switch (o->b04) {
    case 0:
        if (GCD.bd0cf == 0 || (GV2.bc3 & 8)) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->b0a = 0x13;
        o->b6a = 0;
        o->active = 2;
        if (o->b0c == 0) {
            buf[0] = 0x80;
            buf[1] = 0x1fe;
            buf[2] = 0x10;
            buf[3] = 2;
            FUN_8005f3c0(buf, D_80135AA8);
            o->box0 = 0x18;
            o->box1 = 0x30;
            o->box2 = 0x18;
            o->box3 = 0x32;
        } else {
            o->box0 = 0xa;
            o->box1 = 0x14;
            o->box2 = 0xe;
            o->box3 = 0x36;
        }
        o->d84 = 0;
        o->d88 = D_80135AE8[o->b0c];
        o->d8c = 0;
        if (((unsigned char*)&GCD.bcf0e)[0] == 0) {
            o->step = 0;
            o->w74 = 0x800;
            o->w76 = 0x800;
            o->w78 = 0x800;
        } else {
            if (o->b0c == 0) {
                o->step = 2;
                o->active = 1;
            } else {
                o->step = 1;
                o->active = 2;
            }
            o->w74 = 0x1000;
            o->w76 = 0x1000;
            o->w78 = 0x1000;
        }
        break;
    case 1:
        if (o->b0c != 0) {
            p = (TObj *)o->d90;
            if (p->visible == 0) break;
            o->w74 = p->w74;
            o->w76 = p->w76;
            o->w78 = p->w78;
            dx = (short)(o->d30 >> 16) * o->w74;
            dy = (short)(o->d34 >> 16) * o->w74;
            o->h->raw = p->h->raw + (dx << 4);
            o->y.raw = p->y.raw + (dy << 4);
            o->visible = 1;
            FUN_80018cf0(o);
            switch (o->step) {
            case 0:
                if (o->b6a) o->step++;
                break;
            case 1:
                if (o->b0c == 2) {
                    o->d88 -= 0x20;
                    if (o->d88 < -0x638) {
                        o->d88 = -0x638;
                        o->step++;
                    }
                } else {
                    o->d88 += 0x20;
                    if (o->d88 > 0x638) {
                        o->d88 = 0x638;
                        o->step++;
                    }
                }
                break;
            case 2:
                o->active = 1;
                break;
            }
        } else {
            FUN_800202b4(o);
            switch (o->step) {
            case 0:
                if (onscreen(o)) o->step++;
                break;
            case 1:
                evilPigBagPortalOpen(o);
                break;
            case 2:
                evilPigBagPortalEnter(o);
                break;
            }
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
