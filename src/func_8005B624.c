// FUNC 8005b624 784 MAIN0
// MATCHING 8005b624 784
#include "TOBJ.H"
extern TObj *ObjAlloc(void);
extern unsigned char D_80080524[];
extern unsigned char D_800805EC[];
extern int D_80080504[];
typedef struct { short a, b; } Pair;
extern Pair D_80080704[];

void func_8005B624(int k, int b)
{
    int d[8];
    int n, q1, q2, q3, q4, q5, q6, q7, q8;
    int c, i;
    TObj *o;

    if (b == 0)
        n = D_80080504[D_80080524[k]];
    else
        n = D_80080504[D_800805EC[k]];
    if (n == 0)
        return;
    q1 = n / 10;
    q2 = q1 / 10;
    q3 = q2 / 10;
    q4 = q3 / 10;
    d[0] = n - q1 * 10;
    q5 = q4 / 10;
    d[1] = q1 - q2 * 10;
    d[2] = q2 - q3 * 10;
    q6 = q5 / 10;
    d[3] = q3 - q4 * 10;
    d[4] = q4 - q5 * 10;
    q7 = q6 / 10;
    d[5] = q5 - q6 * 10;
    d[6] = q6 - q7 * 10;
    q8 = q7 / 10;
    d[7] = q7 - q8 * 10;
    if (n < 10) c = 1;
    else if (n < 100) c = 2;
    else if (n < 1000) c = 3;
    else if (n < 10000) c = 4;
    else if (n < 100000) c = 5;
    else if (n < 1000000) c = 6;
    else if (n < 10000000) c = 7;
    else c = 8;
    for (i = 0; i < c; i++) {
        o = ObjAlloc();
        if (o != 0) {
            o->active = 1;
            o->type = 0x22;
            o->subtype = b;
            o->b0c = d[i];
            o->w08 = 0x7d16;
            o->b0d = 1;
            o->animFrame = 1;
            o->a.raw = (c * 8 + 160 - i * 16) << 16;
            o->b0f = 0;
            o->y.raw = 0xa00000;
            o->category |= 0x80;
            o->b.raw = 0;
            o->wb4 = D_80080704[b].a;
            o->wb6 = D_80080704[b].b;
        }
    }
}
