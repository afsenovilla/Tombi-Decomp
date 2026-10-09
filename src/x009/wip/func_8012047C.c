/* score 14: only r/x1 registers swapped (game r in a3, x1 in t0; ours r t0, x1 a3): -dg shows x1 (4 refs/40 insns) outranks r (3 refs/17 insns). Tried: all orders of the x1/x2/f block, int/short types of l r x1 x2, if/else orders, reusing other vars for l/r/x1. Structure (o34): case 0 return gives the game switch tree, per-case sx block locals put o in s0, chk written per case as if (!f) return; goto hit2. */
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
    if (o->b9c ? o->velX >= 0 : o->velH >= 0) {
        l = 8;
        r = -8;
    } else {
        l = -8;
        r = 8;
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
        if (c && func_80121434(o, x1, d2 + 8, 0, e->subtype)) o->ba6 = 2;
        {
            short sx = a;
            if (func_801214D0(o, sx, d - 0xc, e->subtype)) goto hit;
            if (!func_801215C0(o, sx, d + o->box2, e->subtype)) return;
            goto hit2;
        }
    case 2:
        if (c && func_80121434(o, x2, d2 + 8, 1, e->subtype)) o->ba6 = 3;
        {
            short sx = b;
            if (func_801214D0(o, sx, d - 0xc, e->subtype)) goto hit;
            if (!func_801215C0(o, sx, d + o->box2, e->subtype)) return;
            goto hit2;
        }
    case 3:
        {
            short sx, sy;
            if (c) {
                sy = d2 + 8;
                if (func_80121434(o, x1, sy, 0, e->subtype)) o->ba6 = 2;
                else if (func_80121434(o, x2, sy, 1, e->subtype)) o->ba6 = 3;
            }
            sx = a;
            sy = d - 0xc;
            if (func_801214D0(o, sx, sy, e->subtype) || func_801214D0(o, sx, sy, e->subtype)) {
            hit:
                if (o->velY < 0) o->velY = 0;
                D_1F80019E = 0;
                return;
            }
            if (func_801215C0(o, sx, d + o->box2, e->subtype)) goto hit2;
            if (func_801215C0(o, b, d + o->box2, e->subtype)) goto hit2;
        }
        return;
    default:
        return;
    }
hit2:
    FUN_8003fb90(o);
    o->h->whole += e->velX;
    D_1F80019E = 0;
    o->velY = 0;
}
