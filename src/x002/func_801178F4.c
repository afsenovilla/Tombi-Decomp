// FUNC 801178f4 1016 X002
// MATCHING 801178f4 1016
#include "TOBJ.H"

typedef struct { short x, y, w, h; } RECT;
extern int LoadImage(RECT *, void *);
extern unsigned char D_8009D0CD;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CF0F[];
extern unsigned char D_8009C93A;
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern char D_8011C728[];
extern short D_8011C768[];
extern void ObjListPush_1F800224(TObj *o);
extern void ObjCullRegister(TObj *o);
extern void ObjFreeDup(TObj *o);
extern void func_801174B4(TObj *o);
extern void func_80117644(TObj *o);

static __inline__ int onScreen(TObj *o)
{
    if (D_8009C93A == 0) return 0;
    if ((unsigned short)(o->h->p.whole - D_1F800176) >= 0x12d) return 0;
    return (unsigned short)(D_1F800186 - (o->y.p.whole + 0x10)) < 0xc9;
}

void func_801178F4(TObj *o)
{
    RECT r;
    TObj *p;
    short v;
    int m1, m2;

    switch (o->b04) {
    case 0:
        if (D_8009D0CD == 0 || (D_8009D2C3 & 1)) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->b0a = 0x13;
        o->b6a = 0;
        o->active = 2;
        if (o->b0c == 0) {
            r.x = 0x80;
            r.y = 0x1fc;
            r.w = 0x10;
            r.h = 2;
            LoadImage(&r, D_8011C728);
            o->box0 = 0x18;
            o->box1 = 0x30;
            o->box2 = 0x18;
            o->box3 = 0x32;
        } else {
            o->box0 = 10;
            o->box1 = 0x14;
            o->box2 = 0xe;
            o->box3 = 0x36;
        }
        o->d84 = 0;
        o->d88 = D_8011C768[o->b0c];
        o->d8c = 0;
        if (D_8009CF0F[0] == 0) {
            o->step = 0;
            v = 0x800;
        } else {
            if (o->b0c == 0) {
                o->step = 2;
                o->active = 1;
            } else {
                o->step = 1;
                o->active = 2;
            }
            v = 0x1000;
        }
        o->w74 = v;
        o->w76 = v;
        o->w78 = v;
        break;
    case 1:
        if (o->b0c != 0) {
            p = (TObj *)o->d90;
            if (p->visible == 0) break;
            o->w74 = p->w74;
            o->w76 = p->w76;
            o->w78 = p->w78;
            m1 = *(short *)((char *)o + 0x32) * o->w74;
            m2 = *(short *)((char *)o + 0x36) * o->w74;
            o->h->raw = p->h->raw + (m1 << 4);
            o->y.raw = p->y.raw + (m2 << 4);
            o->visible = 1;
            ObjListPush_1F800224(o);
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
            ObjCullRegister(o);
            switch (o->step) {
            case 0:
                if (onScreen(o)) o->step++;
                break;
            case 1:
                func_801174B4(o);
                break;
            case 2:
                func_80117644(o);
                break;
            }
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
