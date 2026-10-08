// FUNC 8003509c 128 MAIN0
// MATCHING 8003509c 128
// FLAGS -O1 -G0
#include "TOBJ.H"
extern volatile int DAT_1f800340[];
extern unsigned char DAT_800a6047;
extern unsigned short DAT_800a604a, DAT_800a604e, DAT_800a6052;
void FUN_8003509c(TObj *o, int n)
{
    volatile int *p;
    int q, a, b;
    unsigned char c;
    unsigned short s;
    p = DAT_1f800340;
    n <<= 2;
    q = p[0];
    a = p[0];
    b = *(int *)(q + n + 4);
    o->ba4 = 1;
    o->da0 = a + b;
    c = DAT_800a6047;
    o->b0a = 0x11;
    o->b0f = c - 2;
    o->a.p.whole = DAT_800a604a;
    o->y.p.whole = DAT_800a604e;
    s = DAT_800a6052;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    o->b.p.whole = s;
}
