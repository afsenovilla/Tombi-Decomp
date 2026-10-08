// FUNC 80119d2c 288 X009
// MATCHING 80119d2c 288
typedef struct { short x, y, z, pad; } P8;
typedef struct { short i, b6, b8, ba; int bc, c0; } X;
extern P8 D_8012AC88[];
extern int SquareRoot0(int);
extern short ratan2(int, int);

void func_80119D2C(char *o)
{
    X *x = (X *)(o + 0xb4);
    int dx, dz, dy, d;

    for (;;) {
        dx = D_8012AC88[x->i + 1].x - D_8012AC88[x->i].x;
        dz = D_8012AC88[x->i + 1].z - D_8012AC88[x->i].z;
        dy = D_8012AC88[x->i + 1].y - D_8012AC88[x->i].y;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d != 0) {
            x->c0 = d << 8;
            if (x->bc < (d << 8)) break;
            x->bc -= d << 8;
        }
        x->i++;
    }
    x->ba = ratan2(dy, d);
    x->b8 = ratan2(dz, dx);
}
