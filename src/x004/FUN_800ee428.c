// FUNC 800ee428 44 X004
// MATCHING 800ee428 44
// FLAGS -O1 -G0
typedef struct TQ { char p0[0x20]; short f20; char p1[0x2e - 0x22]; unsigned short f2e; char p2[0x84 - 0x30]; int f84; int f88; int f8c; } TQ;
void FUN_800ee428(TQ *o)
{
    int a = 16;
    unsigned short s = o->f2e;
    o->f20 = 10;
    o->f84 = 0;
    if (s & 1)
        a = 0xf0;
    o->f88 = a;
    o->f8c = 0;
}
