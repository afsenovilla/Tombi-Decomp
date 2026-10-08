// FUNC 8004065c 140 MAIN0
// MATCHING 8004065c 140
typedef struct {
    char pad[0x44];
    short *p44;
} O8004065C;

extern int func_8003F200(int, int);
extern short func_800402EC(O8004065C *, int, int, short);

short func_8004065C(O8004065C *o, short a, short b, int c)
{
    *(int *)0x1F800278 = func_8003F200(a, o->p44[1]);
    return func_800402EC(o, a, b, (c & 1) | 0xff00);
}
