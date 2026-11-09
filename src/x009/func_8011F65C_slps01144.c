// VERSION SLPS_01144 x09 8011F65C 1296
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct { unsigned short frac, whole; } UF;
typedef struct O {
    unsigned char active, visible, type, subtype;
    char p04[0x16 - 4];
    unsigned short y;
    char p18[0x40 - 0x18];
    UF *h, *d;
    char p48[0x6c - 0x48];
    unsigned short box0;
    short box1;
    unsigned short box2;
    short box3;
    char p74[0x7c - 0x74];
    short velX, velY, velH;
    char p82[0x9c - 0x82];
    unsigned char b9c, b9d, b9e;
    char p9f[0xa6 - 0x9f];
    unsigned char ba6;
    char pa7[0xaa - 0xa7];
    unsigned char baa;
} O;

extern short LIST_COUNTER;
extern short func_801206A0_slps01144(O *, short, short, int, unsigned char);
extern short func_8012073C_slps01144(O *, short, short, unsigned char);
extern short func_8012082C_slps01144(O *, short, short, unsigned char);
extern void func_80040760(O *);

void func_8011F65C_slps01144(O *o, O *e)
{
    unsigned short a, b, d;
    int l, r, t;
    short f;
    short c;
    int x1, x2, d2;

    if (o->b9e) return;
    a = o->d->whole - e->d->whole + 0x2d;
    if ((unsigned short)a >= 0x5b) return;
    b = o->h->whole - e->h->whole + (o->box0 + e->box0);
    if ((unsigned short)b >= e->box1 + o->box1) return;
    d = o->y - e->y + (e->box2 + o->box2);
    if ((unsigned short)d >= e->box3 + o->box3) return;
    if (o->b9c) {
        if (o->velX >= 0) { l = 8; r = -8; } else { l = -8; r = 8; }
    } else {
        if (o->velH >= 0) { l = 8; r = -8; } else { l = -8; r = 8; }
    }
    x1 = o->h->whole + l - (t = e->h->whole - e->box0);
    a = x1;
    x2 = o->h->whole + r - t;
    f = (unsigned short)a < e->box1;
    b = x2;
    if ((unsigned short)b < e->box1) f |= 2;
    d2 = o->y - (e->y + ((unsigned short)e->box3 - e->box2));
    d = d2;
    c = o->baa == 0;
    switch (f) {
    case 0:
        return;
    case 1:
        {
            short sx = x1;
            if (func_8012082C_slps01144(o, sx, d + o->box2, e->subtype)) {
                func_80040760(o);
                o->h->whole += e->velX;
                LIST_COUNTER = 0;
                o->velY = 0;
            } else if (func_8012073C_slps01144(o, sx, d2 - 0xc, e->subtype)) {
                if (o->velY < 0) o->velY = 0;
                LIST_COUNTER = 0;
            }
        }
        if (c && func_801206A0_slps01144(o, a, d + 8, 0, e->subtype)) o->ba6 = 2;
        return;
    case 2:
        {
            short sx = x2;
            if (func_8012082C_slps01144(o, sx, d + o->box2, e->subtype)) {
                func_80040760(o);
                o->h->whole += e->velX;
                LIST_COUNTER = 0;
                o->velY = 0;
            } else if (func_8012073C_slps01144(o, sx, d2 - 0xc, e->subtype)) {
                if (o->velY < 0) o->velY = 0;
                LIST_COUNTER = 0;
            }
        }
        if (c && func_801206A0_slps01144(o, b, d + 8, 1, e->subtype)) o->ba6 = 3;
        return;
    case 3:
        {
            short sx = x1, sy;
            if (func_8012082C_slps01144(o, sx, d + o->box2, e->subtype) || func_8012082C_slps01144(o, x2, d + o->box2, e->subtype)) {
                func_80040760(o);
                o->h->whole += e->velX;
                LIST_COUNTER = 0;
                o->velY = 0;
            } else {
                sy = d2 - 0xc;
                if (func_8012073C_slps01144(o, sx, sy, e->subtype) || func_8012073C_slps01144(o, sx, sy, e->subtype)) {
                    if (o->velY < 0) o->velY = 0;
                    LIST_COUNTER = 0;
                }
            }
            if (c) {
                sy = d + 8;
                if (func_801206A0_slps01144(o, a, sy, 0, e->subtype)) o->ba6 = 2;
                else if (func_801206A0_slps01144(o, b, sy, 1, e->subtype)) o->ba6 = 3;
            }
        }
        return;
    }
}
#endif
