// VERSION SLPM_80165 slpm80165 8003CE40 132
#if defined(VERSION_SLPM_80165)
// Demo-only collision probe: no column below 0, else load the plane list at x and run func_8003CB64.
extern void *func_8003C524(int, int);
extern void *D_1F800278;
extern short func_8003CB64(char *, short, short);
short func_8003CE40_slpm80165(char *o, short x, short y)
{
    if (x < 0) return 0;
    D_1F800278 = func_8003C524(x, (*(short **)(o + 0x44))[1]);
    return func_8003CB64(o, x, y);
}
#endif
