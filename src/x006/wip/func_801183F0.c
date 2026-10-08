// FUNC 801183f0 256 X006
/* score 7: only the path-point pointers differ: the game computes base+idx*8 twice (addu a0,v1,v0; addu a1,v1,v0)
   from a temp base; every C form tried either CSEs them into one register or copies a=b (move a1,a0).
   Tried: a/b from pa8[i] / &t[i] / int casts / short* view / function-scope vars. Same pattern in func_801184F0. */
typedef struct { short x, y, z, pad; } P8;
typedef struct { short b4, b6, b8, ba; int bc, c0, c4; } X;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x2c - 8];
    unsigned short w2c;
    char p2e[0x76 - 0x2e];
    short w76;
    char p78[0xa8 - 0x78];
    P8 *pa8;
    char pac[0xb4 - 0xac];
    X x;
} O;
extern int SquareRoot0(int);
extern short ratan2(int, int);

void func_801183F0(O *o)
{
    X *x = &o->x;
    int dx, dz, dy, d;
    P8 *a, *b;
    int i;

    for (;;) {
        i = o->w2c;
        a = o->pa8;
        b = a;
        a += i;
        b += i;
        dx = a[1].x - b->x;
        dz = a[1].z - b->z;
        dy = a[1].y - b->y;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d != 0) {
            x->c0 = d << 8;
            if (x->bc < (d << 8)) break;
            x->bc -= d << 8;
        }
        o->w2c++;
    }
    x->ba = ratan2(dy, d);
    o->w76 = dy;
    x->b8 = ratan2(dz, dx);
}
