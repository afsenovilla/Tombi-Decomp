// FUNC 801184f0 380 X006
/* score 32: same two-register path-point problem as wip/func_801183F0 (now `base - -i` form: two addu, but base ties with a);
   also the game computes o+0xb4 into a0 early and copies it to s2 only on the backward path (x/s2 vs s3 swap). */
typedef struct { short x, y, z, pad; } P8;
typedef struct { short b4, b6, b8, ba; int bc; unsigned int c0; int c4; } X;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x2c - 8];
    unsigned short w2c;
    char p2e[0xa8 - 0x2e];
    P8 *pa8;
    char pac[0xb4 - 0xac];
    X x;
} O;
extern int SquareRoot0(int);
extern short ratan2(int, int);
extern void func_801183F0(O *);

void func_801184F0(O *o)
{
    X *x = &o->x;
    int dx, dz, dy, d;
    int v;
    P8 *a, *b;
    int i;

    v = x->bc + x->b6;
    x->bc = v;
    if (x->c0 < (unsigned int)v) {
        if (v < 0) {
            if (o->w2c == 0) {
                x->bc = 0;
                return;
            }
            x->c4 -= x->c0;
            o->w2c--;
            for (;;) {
                i = o->w2c << 3;
                b = (P8 *)((int)o->pa8 - -i);
                a = (P8 *)((int)o->pa8 + i);
                dx = a[1].x - b->x;
                dz = a[1].z - b->z;
                dy = a[1].y - b->y;
                d = SquareRoot0(dx * dx + dz * dz);
                if (d != 0) {
                    x->c0 = d << 8;
                    x->bc += d << 8;
                    if (x->bc >= 0) break;
                }
                o->w2c--;
            }
            x->ba = ratan2(dy, d);
            x->b8 = ratan2(dz, dx);
        } else {
            x->bc = v - x->c0;
            o->w2c++;
            func_801183F0(o);
            x->c4 += x->c0;
        }
    }
}
