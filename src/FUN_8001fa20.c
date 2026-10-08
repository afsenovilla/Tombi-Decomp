// FUNC 8001fa20 64 MAIN0
// MATCHING 8001fa20 64
typedef struct O { char p0[0x14]; int y; char p1[0x28-0x18]; short *t; char p2[0x40-0x2c]; int *h; } O;
typedef struct P { short x, y; } P;
void FUN_8001fa20(O *o, unsigned short i)
{
    P *p = (P *)o->t + i;
    int d;
    d = p->x << 8;
    *o->h += d;
    d = p->y << 8;
    o->y += d;
}
