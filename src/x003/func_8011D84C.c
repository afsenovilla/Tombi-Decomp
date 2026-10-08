// FUNC 8011d84c 1016 X003
// MATCHING 8011d84c 1016
#include "TOBJ.H"
extern unsigned char D_8009D0CF;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CF0E[];
extern unsigned char D_8009C93A;
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern char D_80135AA8[];
extern short D_80135AE8[];
extern void FUN_8005f3c0(short *, char *);
extern void FUN_80018cf0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);
extern void func_8011D40C(TObj *);
extern void func_8011D59C(TObj *);

static __inline__ int onscreen(TObj *o)
{
    if (D_8009C93A == 0) return 0;
    if ((unsigned short)(o->h->p.whole - D_1F800176) >= 0x12d) return 0;
    return (unsigned short)(D_1F800186 - (o->y.p.whole + 0x10)) < 0xc9;
}

void func_8011D84C(TObj *o)
{
    short buf[4];
    TObj *p;
    int dx, dy;

    switch (o->b04) {
    case 0:
        if (D_8009D0CF == 0 || (D_8009D2C3 & 8)) {
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
        if (D_8009CF0E[0] == 0) {
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
                func_8011D40C(o);
                break;
            case 2:
                func_8011D59C(o);
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
