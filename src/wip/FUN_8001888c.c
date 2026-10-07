// FUNC 8001888c 84 MAIN0
typedef struct O { int a, b, c, d; char p0[0x1c - 0x10]; unsigned char cat; char p1[0x9c - 0x1d]; char e, f; } O;
extern struct { char p0[0x20c]; int *sp; char p1[0x23a - 0x210]; unsigned short cnt; } S1F;
void FUN_8001888c(O *o)
{
    o->a = 0;
    o->b = 0;
    o->c = 0;
    o->d = 0;
    o->e = 0;
    o->f = 0;
    o->cat &= 0x7f;
    S1F.cnt++;
    S1F.sp--;
    *S1F.sp = (int)o;
}
