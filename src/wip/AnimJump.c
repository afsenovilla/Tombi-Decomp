// FUNC 8001fe94 44 MAIN0
typedef struct D { int a; short b; unsigned short v; } D;
typedef struct A { char p[0x24]; D *d; char q[4]; unsigned short dur; } A;
void AnimJump(A *o, short n)
{
    unsigned short v = o->d[n].v;
    o->d = o->d + n;
    o->dur = v & 0x3fff;
}
