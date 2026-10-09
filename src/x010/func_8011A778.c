// FUNC 8011a778 1016 X010
// MATCHING 8011a778 1016
#include "TOBJ.H"

typedef struct { short x, y, w, h; } RECT;

extern unsigned char D_8009D0CA, D_8009D2C3, D_8009CF0C[], D_8009C93A;
extern char D_8012F2A8[];
extern short D_8012F2E8[];
extern unsigned short D_1F800176, D_1F800186;
extern void FUN_8005f3c0(RECT *, char *);
extern void FUN_80018cf0(TObj *);
extern int FUN_800202b4(TObj *);
extern void func_8011A338(TObj *);
extern void func_8011A4C8(TObj *);
extern void FUN_80018838(TObj *);

static __inline__ int near(TObj *o)
{
    if (D_8009C93A == 0)
        return 0;
    if ((unsigned short)(o->h->p.whole - D_1F800176) >= 0x12d)
        return 0;
    return (unsigned short)(D_1F800186 - (o->y.p.whole + 0x10)) < 0xc9;
}

void func_8011A778(TObj *o)
{
    RECT r;
    TObj *p;
    int dx, dy;

    switch (o->b04) {
    case 0:
        if (D_8009D0CA == 0 || (D_8009D2C3 & 0x10)) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->b0a = 0x13;
        o->b6a = 0;
        o->active = 2;
        if (o->b0c == 0) {
            r.x = 0x80;
            r.y = 0x1fe;
            r.w = 0x10;
            r.h = 2;
            FUN_8005f3c0(&r, D_8012F2A8);
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
        o->d88 = D_8012F2E8[o->b0c];
        o->d8c = 0;
        if (D_8009CF0C[0] == 0) {
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
            if (p->visible == 0)
                break;
            o->w74 = p->w74;
            o->w76 = p->w76;
            o->w78 = p->w78;
            dx = ((Fix16 *)&o->d30)->p.whole * o->w74;
            dy = ((Fix16 *)&o->d34)->p.whole * o->w74;
            o->h->raw = p->h->raw + (dx << 4);
            o->y.raw = p->y.raw + (dy << 4);
            o->visible = 1;
            FUN_80018cf0(o);
            switch (o->step) {
            case 0:
                if (o->b6a)
                    o->step++;
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
                if (near(o))
                    o->step++;
                break;
            case 1:
                func_8011A338(o);
                break;
            case 2:
                func_8011A4C8(o);
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
