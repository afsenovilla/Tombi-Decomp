// FUNC 80041ca8 120 MAIN0
typedef struct O { char p[0x44]; short *q; } O;
extern int FUN_8003f200(int, int);
extern int FUN_800416b8(O *, int, int, int);
extern int DAT_1f800278;
short FUN_80041ca8(O *o, short b, int c)
{
short *q = o->q; DAT_1f800278 = FUN_8003f200(b, q[1]); return FUN_800416b8(o, b, (short)c, -1);
}
