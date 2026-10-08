// FUNC 80121434 156 X009
// MATCHING 80121434 156
typedef struct { char pad[0x44]; short *p44; } O;
extern char *func_8003F1BC(short, unsigned char);
extern short func_800402EC(O *, int, int, short);

short func_80121434(O *o, short a, short b, int c, unsigned char d)
{
    *(int *)0x1F800278 = (int)func_8003F1BC(a, d);
    return func_800402EC(o, a, b, (d << 8) | (c & 1));
}
