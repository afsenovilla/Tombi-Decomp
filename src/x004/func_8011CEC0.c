// FUNC 8011cec0 1016 X004
// MATCHING 8011cec0 1016
#include "TOBJ.H"
typedef struct { short x, y, w, h; } RECT;
extern unsigned char D_8009D0CC, D_8009D2C3, D_8009C93A;
extern unsigned char D_8009CF11[];
extern char D_80130CD4[];
extern short D_80130D14[];
extern unsigned short D_1F800176, D_1F800186;
extern void FUN_8005f3c0(RECT *, void *);
extern void FUN_80018cf0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);
extern void func_8011CA80(TObj *);
extern void func_8011CC10(TObj *);

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

static __inline__ int inview(TObj *o)
{
    if (!D_8009C93A) return 0;
    if ((unsigned short)(o->h->p.whole - D_1F800176) >= 0x12d) return 0;
    return (unsigned short)(D_1F800186 - (o->y.p.whole + 0x10)) < 0xc9;
}

void func_8011CEC0(TObj *o)
{
    TObj *p;
    short k;
    RECT r;
    int x, y;
    short m;

    switch (o->b04) {
    case 0:
        if (!D_8009D0CC || (D_8009D2C3 & 4)) {
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
            FUN_8005f3c0(&r, D_80130CD4);
            setbox(o, 0x18, 0x30, 0x18, 0x32);
        } else {
            setbox(o, 10, 0x14, 0xe, 0x36);
        }
        o->d84 = 0;
        o->d88 = D_80130D14[o->b0c];
        o->d8c = 0;
        if (D_8009CF11[0] == 0) {
            o->step = 0;
            k = 0x800;
        } else {
            if (o->b0c == 0) {
                o->step = 2;
                o->active = 1;
            } else {
                o->step = 1;
                o->active = 2;
            }
            k = 0x1000;
        }
        o->w74 = k;
        o->w76 = k;
        o->w78 = k;
        break;
    case 1:
        if (o->b0c) {
            p = (TObj *)o->d90;
            if (!p->visible) break;
            *(volatile short *)&o->w74 = p->w74;
            m = o->w74;
            x = ((Fix16 *)&o->d30)->p.whole * m;
            o->w76 = p->w76;
            y = ((Fix16 *)&o->d34)->p.whole * m;
            o->w78 = p->w78;
            o->h->raw = p->h->raw + (x << 4);
            o->y.raw = p->y.raw + (y << 4);
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
                    if (o->d88 >= 0x639) {
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
                if (inview(o)) o->step++;
                break;
            case 1:
                func_8011CA80(o);
                break;
            case 2:
                func_8011CC10(o);
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
