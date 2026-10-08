// r9 wip: score 40. Whole function 8012127c..8012160c (splat splits it into 3: func_8012127C/func_801212A0/func_801213D0). Only the prologue differs: game keeps p in a3 as 'addiu a3,0x1c; lhu t0,0(a3); addiu a3,4', we fold it into lhu 28(v1) + p=v1+32 (so p/n regs swap).
// FUNC 8012127c 912 X000
#include "TOBJ.H"
typedef struct {
    unsigned char c[0x36];
    short idx;
    unsigned char pad[0x18];
    unsigned char z[3];
    unsigned char pad2;
} P;
extern unsigned char D_80138D74[][8][12];

void func_8012127C(TObj *o)
{
    short n, k, r;
    P *p;
    p = (P *)((unsigned char *)o->da0 + 0x1c);
    n = *(unsigned short *)p;
    p = (P *)((unsigned char *)p + 4);
    switch (o->state) {
    case 0:
        o->state++;
        o->ba4 = 0;
        o->timer = 0;
        do {
            r = (0x20 - n) >> 3;
            k = (0x20 - n) & 7;
            p->z[0] = 0;
            p->z[1] = 0;
            p->z[2] = 0;
            p->c[0] = D_80138D74[r][k][0];
            p->c[1] = D_80138D74[r][k][1];
            p->c[2] = D_80138D74[r][k][2];
            p->c[4] = D_80138D74[r][k][3];
            p->c[5] = D_80138D74[r][k][4];
            p->c[6] = D_80138D74[r][k][5];
            p->c[8] = D_80138D74[r][k][6];
            p->c[9] = D_80138D74[r][k][7];
            p->c[10] = D_80138D74[r][k][8];
            p->c[12] = D_80138D74[r][k][9];
            p->c[13] = D_80138D74[r][k][10];
            p->c[14] = D_80138D74[r][k][11];
            p->idx = k;
            p++;
        } while (--n);
        break;
    case 1:
        do {
            r = (0x20 - n) >> 3;
            p->idx = (p->idx + 1) & 7;
            p->c[0] = D_80138D74[r][p->idx][0];
            p->c[1] = D_80138D74[r][p->idx][1];
            p->c[2] = D_80138D74[r][p->idx][2];
            p->c[4] = D_80138D74[r][p->idx][3];
            p->c[5] = D_80138D74[r][p->idx][4];
            p->c[6] = D_80138D74[r][p->idx][5];
            p->c[8] = D_80138D74[r][p->idx][6];
            p->c[9] = D_80138D74[r][p->idx][7];
            p->c[10] = D_80138D74[r][p->idx][8];
            p->c[12] = D_80138D74[r][p->idx][9];
            p->c[13] = D_80138D74[r][p->idx][10];
            p->c[14] = D_80138D74[r][p->idx][11];
            p++;
        } while (--n);
        break;
    }
}
