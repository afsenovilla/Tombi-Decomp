// FUNC 80025f40 192 MAIN0
typedef struct { char c[6]; } B;
typedef struct { short p0; unsigned short a; unsigned short b; unsigned char d; unsigned char e; } G;
extern B DAT_80010238;
extern G g;
extern void FUN_80069410(int, void *, int, int);

void FUN_80025f40(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
    B buf;
    buf = DAT_80010238;
    if (g.a != 0 && g.d == 0 && g.b == 0) {
        ((unsigned char *)&g.b)[1] = c;
        *(unsigned char *)&g.b = b;
        FUN_80069410(a, &g.b, 2, d);
        g.e = d;
        g.d = 1;
    }
}
