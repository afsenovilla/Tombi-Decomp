// FUNC 8006adfc 32 MAIN0
// MATCHING 8006adfc 32
typedef struct { char p0[0x24]; char b24; char p1[0x2c-0x25]; char *p2c; char p2[0x36-0x30]; char b36; char b37; } TO;

void FUN_8006adfc(TO *o, int a)
{
    o->b37 = 0x46;
    o->p2c = &o->b24;
    o->b24 = a;
    o->b36 = 1;
}
