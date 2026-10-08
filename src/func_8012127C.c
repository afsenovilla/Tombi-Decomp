// FUNC 8012127c 912 X000
// MATCHING 8012127c 912
#include "TOBJ.H"
typedef struct {
    unsigned char c[0x36];
    short phase;
    char p38[0x50 - 0x38];
    unsigned char z50, z51, z52;
    char p53;
} E;
typedef struct {
    char p0[0x1c];
    short n;
    char p1e[2];
    E e[1];
} H;
extern unsigned char D_80138D74[][8][12];

void func_8012127C(TObj *o)
{
    short n;
    E *e;
    int ph, g;

    e = (E *)o->da0;
    e = (E *)((char *)e + 0x1c);
    n = *(unsigned short *)e;
    e = (E *)((char *)e + 4);

    switch (o->state) {
    case 0:
        o->state++;
        o->ba4 = 0;
        o->timer = 0;
        do {
            g = n << 16;
            g >>= 16;
            g = 0x20 - g;
            ph = (unsigned short)(0x20 - n) & 7;
            e->z50 = 0;
            e->z51 = 0;
            e->z52 = 0;
            e->c[0] = D_80138D74[g >> 3][ph][0];
            e->c[1] = D_80138D74[g >> 3][ph][1];
            e->c[2] = D_80138D74[g >> 3][ph][2];
            e->c[4] = D_80138D74[g >> 3][ph][3];
            e->c[5] = D_80138D74[g >> 3][ph][4];
            e->c[6] = D_80138D74[g >> 3][ph][5];
            e->c[8] = D_80138D74[g >> 3][ph][6];
            e->c[9] = D_80138D74[g >> 3][ph][7];
            e->c[10] = D_80138D74[g >> 3][ph][8];
            e->c[12] = D_80138D74[g >> 3][ph][9];
            e->c[13] = D_80138D74[g >> 3][ph][10];
            e->c[14] = D_80138D74[g >> 3][ph][11];
            e->phase = ph;
            e++;
        } while (--n != 0);
        break;
    case 1:
        do {
            g = (0x20 - (short)n) >> 3;
            e->phase = (e->phase + 1) & 7;
            e->c[0] = D_80138D74[g][e->phase][0];
            e->c[1] = D_80138D74[g][e->phase][1];
            e->c[2] = D_80138D74[g][e->phase][2];
            e->c[4] = D_80138D74[g][e->phase][3];
            e->c[5] = D_80138D74[g][e->phase][4];
            e->c[6] = D_80138D74[g][e->phase][5];
            e->c[8] = D_80138D74[g][e->phase][6];
            e->c[9] = D_80138D74[g][e->phase][7];
            e->c[10] = D_80138D74[g][e->phase][8];
            e->c[12] = D_80138D74[g][e->phase][9];
            e->c[13] = D_80138D74[g][e->phase][10];
            e->c[14] = D_80138D74[g][e->phase][11];
            e++;
        } while (--n != 0);
        break;
    }
}
