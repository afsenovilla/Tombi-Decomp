// FUNC 800386f4 236 MAIN0
// wip r8: score 22. Fails: game reloads D_8009F0F0 (lui/lw) before the m==0 branch and addiu sp sits after first lbu; volatile reads are guesses.
typedef struct { char p[0x89]; unsigned char r; unsigned short i; char q[0x1090 - 0x8c]; int t[1]; } O;
extern O *D_8009F0F0;
extern unsigned char *D_8009D60C;

static __inline__ int rd4(unsigned char *p)
{
    int s;
    int k;
    for (k = 0; k < 4; k++) ((char *)&s)[k] = *p++;
    return s;
}

void func_800386F4(unsigned char m)
{
    O *o = D_8009F0F0;
    int a, b;
    unsigned char *p;
    a = *(volatile int *)&o->t[D_8009D60C[o->i + 1]];
    p = D_8009D60C + *(volatile unsigned short *)&o->i + 2;
    if (m == 0) {
        b = D_8009F0F0->t[*p];
    } else {
        b = rd4(p);
    }
    if (a == b)
        o->r = 0;
    else if (a < b)
        o->r = 1;
    else
        o->r = 2;
    if (m == 0)
        o->i += 3;
    else
        o->i += 6;
}
