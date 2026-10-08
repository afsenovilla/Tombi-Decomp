// FUNC 800408d8 116 MAIN0
// MATCHING 800408d8 116
typedef struct O { char p0[0x44]; short *q; } O;
extern int FUN_8003f200(int, int);
extern int FUN_800406e8(O *, int, int);
short FUN_800408d8(O *o, short a, int b)
{
    *(int *)0x1f800278 = FUN_8003f200(a, o->q[1]);
    return FUN_800406e8(o, a, (short)b);
}
