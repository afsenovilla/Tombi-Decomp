// FUNC 8001888c 84 MAIN0
typedef struct O { int a, b, c, d; char p0[0x1c - 0x10]; unsigned char cat; char p1[0x9c - 0x1d]; char e, f; } O;
extern unsigned short DAT_1f80023a;
extern int *DAT_1f80020c;
void FUN_8001888c(O *o)
{
o->cat &= 0x7f; o->e = 0; o->f = 0; o->a = 0; o->b = 0; o->c = 0; o->d = 0;
    DAT_1f80023a++;
    DAT_1f80020c--;
    *DAT_1f80020c = (int)o;
}
