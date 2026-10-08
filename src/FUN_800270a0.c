// FUNC 800270a0 132 MAIN0
// MATCHING 800270a0 132
typedef struct O { char pad[3]; char a; char pad2; unsigned char c; } O;
extern char DAT_a;
extern unsigned char DAT_b;
extern signed char DAT_c;
extern unsigned char DAT_d;
extern short DAT_e;

int FUN_800270a0(O *o, char x, unsigned char y)
{
    char *p = &DAT_a;
    *p = 0;
    if (DAT_b != 0)
        return 0;
    if (DAT_c == 3)
        return 0;
    if (DAT_d == 2)
        return 0;
    DAT_c = 0;
    o->c++;
    o->a = x;
    DAT_e = y;
    *p = 1;
    return 1;
}
