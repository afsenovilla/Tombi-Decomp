// FUNC 8006ae1c 32 MAIN0
typedef struct O { char p0[0x24]; char a; char p1[7]; char *q; char p2[6]; char c; char d; } O;
void FUN_8006ae1c(O *o, char a)
{
    o->d = 0x47;
    o->q = &o->a;
    o->a = a;
    o->c = 1;
}
