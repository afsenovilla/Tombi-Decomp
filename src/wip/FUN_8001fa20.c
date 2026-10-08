// FUNC 8001fa20 64 MAIN0
typedef struct O { char p0[0x14]; int y; char p1[0x28-0x18]; char *t; char p2[0x40-0x2c]; int *h; } O;
void FUN_8001fa20(O *o, unsigned short i)
{
    int off = i << 2;
    short *p = (short *)(o->t + off);
    *o->h += *p << 8;
    o->y += *(short *)((char *)p + 2) << 8;
}
