// FUNC 800ee9cc 112 X008
// MATCHING 800ee9cc 112
typedef struct TO {
    char p0[0x14];
    int y;
    char p1[0x40 - 0x18];
    int *h;
    char p2[0xb2 - 0x44];
    short w2;
    char p3[0xb6 - 0xb4];
    short w6;
} TO;
extern short VX, VY;
extern void g(TO *o, int a, int b);
extern void ObjApplyVelocity(TO *o);

static __inline__ void add(TO *p)
{
    *p->h += VX << 8;
    p->y += VY << 8;
}

void FUN_800ee9cc(TO *o)
{
    g(o, o->w6, o->w2);
    add(o);
    ObjApplyVelocity(o);
}
