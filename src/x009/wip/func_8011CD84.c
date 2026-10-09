/* score 26: only case 1 (b0c != 0) copy block differs: game reloads o->w74 with lh after storing it (sh 0x74; lh 0x32; lh 0x74), ours reuses the stored value (CSE). Tried volatile/raw reads, q=o alias store, S1/C2/S3 struct copies (S3 gives the reload but lwl/swl copy), inline helpers. */
// FUNC 8011cd84 1016 X009
#include "TOBJ.H"

typedef struct { short x, y, w, h; } RECT;
extern unsigned char D_8009D0CE, D_8009D2C3, D_8009CF10[], D_8009C93A;
extern char D_8012B284[];
extern short D_8012B2C4[];
extern unsigned short D_1F800176, D_1F800186;
extern void LoadImage(RECT *, char *);
extern int ObjCullRegister(TObj *);
extern void ObjListPush_1F800224(TObj *);
extern void ObjFreeDup(TObj *);
extern void func_8011C944(TObj *);
extern void func_8011CAD4(TObj *);

#define W32(o) (((short *)&(o)->d30)[1])
#define W36(o) (((short *)&(o)->d34)[1])

static __inline__ int Near(TObj *o)
{
    if (D_8009C93A == 0) return 0;
    if ((unsigned short)(o->h->p.whole - D_1F800176) >= 0x12d) return 0;
    return (unsigned short)(D_1F800186 - (o->y.p.whole + 0x10)) < 0xc9;
}

void func_8011CD84(TObj *o)
{
    unsigned char s = o->b04;
    TObj *e;
    short v;
    int a, b;

    switch (s) {
    case 0:
        if (D_8009D0CE == 0 || (D_8009D2C3 & 0x20)) {
            o->b04 = 3;
            break;
        }
        o->b04 = s + 1;
        o->b0a = 0x13;
        o->b6a = 0;
        o->active = 2;
        if (o->b0c == 0) {
            RECT r;
            r.x = 0x80;
            r.y = 0x1fc;
            r.w = 0x10;
            r.h = 2;
            LoadImage(&r, D_8012B284);
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
        o->d88 = D_8012B2C4[o->b0c];
        o->d8c = 0;
        if (D_8009CF10[0] == 0) {
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
        if (o->b0c) {
            e = (TObj *)o->d90;
            if (e->visible == 0) break;
            o->w74 = e->w74;
            a = W32(o) * o->w74;
            b = W36(o) * o->w74;
            o->w76 = e->w76;
            o->w78 = e->w78;
            o->h->raw = e->h->raw + (a << 4);
            o->y.raw = e->y.raw + (b << 4);
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
                if (Near(o)) o->step++;
                break;
            case 1:
                func_8011C944(o);
                break;
            case 2:
                func_8011CAD4(o);
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
