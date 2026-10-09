/* score 302: game keeps o in s0 and e in s1 (ours s1/s2), and its switch(f) tree tests f==1 first then f<2 (ours roots at 2); also x1 copy via t0. Tried case 0/case 4/default variants, if-shapes for the +-8 offsets (g5 form best), inlined t. */
// FUNC 8012047c 1096 X009
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

extern short D_1F80019E;
extern short func_80121434(O *, short, short, int, unsigned char);
extern short func_801214D0(O *, short, short, unsigned char);
extern short func_801215C0(O *, short, short, int);
extern void FUN_8003fb90(O *);

void func_8012047C(O *o, O *e)
{
    unsigned short a, b, d;
    int l, r, t, f, c;
    short sx, sy;

    if (o->b9e) return;
    a = o->d->whole - e->d->whole + 0x2d;
    if (a >= 0x5b) return;
    b = o->h->whole - e->h->whole + (o->box0 + e->box0);
    if (b >= e->box1 + o->box1) return;
    d = o->y - e->y + (e->box2 + o->box2);
    if (d >= e->box3 + o->box3) return;
    if (o->b9c ? o->velX >= 0 : o->velH >= 0) {
        l = 8;
        r = -8;
    } else {
        l = -8;
        r = 8;
    }
    t = e->h->whole - e->box0;
    a = o->h->whole + l - t;
    b = o->h->whole + r - t;
    f = a < e->box1;
    if (b < e->box1) f |= 2;
    d = o->y - (e->y + (e->box3 - e->box2));
    c = o->baa == 0;
    switch (f) {
    case 1:
        if (c && func_80121434(o, a, d + 8, 0, e->subtype)) o->ba6 = 2;
        sx = a;
        if (func_801214D0(o, sx, d - 0xc, e->subtype)) goto hit;
        goto chk;
    case 2:
        if (c && func_80121434(o, b, d + 8, 1, e->subtype)) o->ba6 = 3;
        sx = b;
        if (func_801214D0(o, sx, d - 0xc, e->subtype)) goto hit;
        goto chk;
    case 3:
        if (c) {
            sy = d + 8;
            if (func_80121434(o, a, sy, 0, e->subtype)) o->ba6 = 2;
            else if (func_80121434(o, b, sy, 1, e->subtype)) o->ba6 = 3;
        }
        sx = a;
        sy = d - 0xc;
        if (func_801214D0(o, sx, sy, e->subtype) || func_801214D0(o, sx, sy, e->subtype)) goto hit;
        if (func_801215C0(o, sx, d + o->box2, e->subtype)) goto hit2;
        sx = b;
    chk:
        if (func_801215C0(o, sx, d + o->box2, e->subtype)) goto hit2;
        return;
    default:
        return;
    }
hit:
    if (o->velY < 0) o->velY = 0;
    D_1F80019E = 0;
    return;
hit2:
    FUN_8003fb90(o);
    o->h->whole += e->velX;
    D_1F80019E = 0;
    o->velY = 0;
}
