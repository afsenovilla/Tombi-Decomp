// JP-only (x00), written from the disassembly
// VERSION SLPS_01144 x00 80120314 1000
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
// probably: a00ApBox50kSparkle helper: drives the sparkle particle entries (phase, z50..z52) of the 50k AP box
#include "TOBJ.H"
typedef struct {
    unsigned char c[0x16];
    short phase;
    char p18[0x50 - 0x18];
    unsigned char z50, z51, z52;
    char p53;
} E;
typedef struct {
    char p0[0x16];
    short phase;
    char p18[0x50 - 0x18];
    unsigned char z50, z51, z52;
    char p53;
} F;
typedef struct {
    char p0[0x1c];
    short n;
    char p1e[2];
    E e[1];
} H;
extern unsigned char D_80137290[][8][12];

void func_80120314(TObj *o)
{
    short n;
    E *e, *g2, *b;
    int ph, g;

    b = (E *)o->da0;
    b = (E *)((char *)b + 0x1c);
    n = *(unsigned short *)b;
    b = (E *)((char *)b + 4);

    switch (o->state) {
    case 0:
        o->state++;
        o->ba4 = 0;
        o->timer = 0;
        e = b; g2 = b;
        do {
            g = n << 16;
            g >>= 16;
            g = 0x20 - g;
            ph = (unsigned short)(0x20 - n) & 7;
            ((F *)g2)->z50 = 0;
            ((F *)g2)->z51 = 0;
            ((F *)g2)->z52 = 0;
            e->c[0] = D_80137290[g >> 3][ph][0];
            e->c[1] = D_80137290[g >> 3][ph][1];
            e->c[2] = D_80137290[g >> 3][ph][2];
            e->c[4] = D_80137290[g >> 3][ph][3];
            e->c[5] = D_80137290[g >> 3][ph][4];
            e->c[6] = D_80137290[g >> 3][ph][5];
            e->c[8] = D_80137290[g >> 3][ph][6];
            e->c[9] = D_80137290[g >> 3][ph][7];
            e->c[10] = D_80137290[g >> 3][ph][8];
            e->c[12] = D_80137290[g >> 3][ph][9];
            e->c[13] = D_80137290[g >> 3][ph][10];
            e->c[14] = D_80137290[g >> 3][ph][11];
            ((F *)g2)->phase = ph;
            g2++;
            e++;
        } while (--n != 0);
        break;
    case 1:
        e = b; g2 = b;
        do {
            g = (0x20 - (short)n) >> 3;
            ((F *)g2)->phase = (((F *)g2)->phase + 1) & 7;
            e->c[0] = D_80137290[g][((F *)g2)->phase][0];
            e->c[1] = D_80137290[g][((F *)g2)->phase][1];
            e->c[2] = D_80137290[g][((F *)g2)->phase][2];
            e->c[4] = D_80137290[g][((F *)g2)->phase][3];
            e->c[5] = D_80137290[g][((F *)g2)->phase][4];
            e->c[6] = D_80137290[g][((F *)g2)->phase][5];
            e->c[8] = D_80137290[g][((F *)g2)->phase][6];
            e->c[9] = D_80137290[g][((F *)g2)->phase][7];
            e->c[10] = D_80137290[g][((F *)g2)->phase][8];
            e->c[12] = D_80137290[g][((F *)g2)->phase][9];
            e->c[13] = D_80137290[g][((F *)g2)->phase][10];
            e->c[14] = D_80137290[g][((F *)g2)->phase][11];
            g2++;
            e++;
        } while (--n != 0);
        break;
    }
}
#endif
