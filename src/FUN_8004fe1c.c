// FUNC 8004fe1c 184 MAIN0
// MATCHING 8004fe1c 184
#include "TOBJ.H"
typedef struct P { char pad[7]; unsigned char code; char pad1[4]; int w0c; char pad2[4]; int w14; } P;
typedef struct Q { int a; int b; unsigned short c; unsigned short pad; unsigned short d; } Q;
extern void func_8005e634(P *p, int v);

void FUN_8004fe1c(TObj *o, P *p, Q *q)
{
    p->code = 0x2d;
    func_8005e634(p, o->b0d >> 7);
    p->w0c = q->a;
    p->w14 = q->b;
    *(unsigned short *)((char *)p + 0x1c) = q->c;
    *(unsigned short *)((char *)p + 0x24) = q->d;
    *(unsigned short *)((char *)p + 0x16) = *(unsigned short *)((char *)p + 0x16) + o->w1e;
    if (o->b0d & 1) {
        *(unsigned short *)((char *)p + 0xe) = o->w08;
    }
}
