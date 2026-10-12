// FUNC 801183f0 256 X006
// MATCHING 801183f0 256
/* Path-point pair: b is reused for the dx argument of the last ratan2 so it is a multi-set (global)
   pseudo and local-alloc cannot tie it with the dying o->pa8 temp (game: pa8 in v1, a/b in a0/a1). */
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
    int a, b;
    int i;

    for (;;) {
        i = o->w2c << 3;
        a = (int)o->pa8 + i;
        b = (int)o->pa8 - -i;
        dx = ((P8 *)a)[1].x - ((P8 *)b)->x;
        dz = ((P8 *)a)[1].z - ((P8 *)b)->z;
        dy = ((P8 *)a)[1].y - ((P8 *)b)->y;
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
    b = dx;
    x->b8 = ratan2(dz, b);
}
