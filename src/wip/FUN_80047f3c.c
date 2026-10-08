// FUNC 80047f3c 408 MAIN0
// wip: logica y frame (char pad[8] en el wrapper del inline) bien; falla la asignacion de registros (juego: a=t0, b=a2)
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))

static __inline__ int push(TObj *a, TObj *b)
{
    int w, dx, p, q;

    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return 0;
    w = U16(b, 0x6c) + (U16(a, 0x6e) - U16(a, 0x6c));
    p = w;
    dx = U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2);
    if ((unsigned short)(dx + w) > S16(b, 0x6e) + (short)U16(a, 0x6e))
        return 0;
    q = dx;
    if ((unsigned short)((U16(a, 0x16) - U16(b, 0x16)) + (U16(b, 0x70) + (U16(a, 0x72) - U16(a, 0x70)))) > (short)U16(a, 0x72) + S16(b, 0x72))
        return 0;
    if ((short)dx < 0) {
        q = -dx;
        p = -w;
    } else {
        p = U16(a, 0x6c) + (S16(b, 0x6e) - U16(b, 0x6c));
    }
    if ((unsigned short)(p - q) < 4) {
        S16(PTR(a, 0x40), 2) = U16(PTR(b, 0x40), 2) + p;
        if ((short)p < 0)
            a->b9d = 2;
        else
            a->b9d = 3;
        return 2;
    }
    if (a->b9c & 1)
        return 0;
    a->y.p.frac = 0;
    a->b69 = 1;
    a->y.p.whole = U16(b, 0x16) - (U16(b, 0x70) + (U16(a, 0x72) - U16(a, 0x70)));
    return 1;
}

int FUN_80047f3c(TObj *a, TObj *b)
{
    char pad[8];

    return push(a, b);
}
