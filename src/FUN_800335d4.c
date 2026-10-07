// FUNC 800335d4 100 MAIN0
// MATCHING 800335d4 100
typedef struct P { short x, y; } P;
typedef struct O { char p0[0x16]; short s16; char p1[0x40 - 0x18]; short *h; } O;
extern unsigned short DAT_800a604e;
extern short *DAT_800a6078;
extern short FUN_8002078c(P, P);
short FUN_800335d4(O *o)
{
    P b, a;
    a.x = o->h[1];
    a.y = o->s16;
    b.x = DAT_800a6078[1];
    b.y = DAT_800a604e;
    return FUN_8002078c(a, b);
}
