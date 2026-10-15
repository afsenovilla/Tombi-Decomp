// JPDEMO slpm80165 8003D4C0 132
// Demo-only collision probe: no column below 0, else load the plane list at x and run func_8003D2B8.
extern void *func_8003C524(int, int);
extern void *D_1F800278;
extern short func_8003D2B8(char *, short, short);
short func_8003D4C0(char *o, short x, short y)
{
    if (x < 0) return 0;
    D_1F800278 = func_8003C524(x, (*(short **)(o + 0x44))[1]);
    return func_8003D2B8(o, x, y);
}
