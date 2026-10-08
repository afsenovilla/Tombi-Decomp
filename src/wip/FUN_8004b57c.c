// FUNC 8004b57c 292 MAIN0
// wip: solo falla abs: el juego guarda d (v1) y r (a2) separados (move a2,v1; negu a2,v1) y luego regs de la 3a condicion
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern short D_1f8003bc;

static __inline__ int chk(TObj *a, TObj *b)
{
    int r, w, dx, d;

    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return 0;
    d = U16(a, 0xe8) - U16(PTR(a, 0x40), 2);
    r = d;
    if ((short)d < 0)
        r = -d;
    dx = U16(a, 0xe8) - U16(PTR(b, 0x40), 2);
    if (a->animFrame & 1)
        w = U16(b, 0x6c) + r;
    else
        w = U16(b, 0x6c);
    if ((unsigned short)(w + dx) > S16(b, 0x6e) + (short)r)
        return 0;
    if ((unsigned short)(U16(b, 0x70) + (U16(a, 0xea) - U16(b, 0x16))) > S16(b, 0x72))
        return 0;
    if (!(a->animFrame & 1))
        D_1f8003bc = -U16(b, 0x6c);
    else
        D_1f8003bc = S16(b, 0x6e) - U16(b, 0x6c);
    return 1;
}

int FUN_8004b57c(TObj *a, TObj *b)
{
    char pad;

    return chk(a, b);
}
