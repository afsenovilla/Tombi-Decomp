// FUNC 8001fa60 40 MAIN0
// MATCHING 8001fa60 40
typedef struct O { char p0[0x14]; int y; char p1[0x28-0x18]; char *t; } O;
void FUN_8001fa60(O *o, unsigned short i)
{
    short *p = (short *)(o->t + (i << 2));
    o->y += p[1] << 8;
}
