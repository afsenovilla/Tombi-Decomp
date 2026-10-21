// FUNC 800eb550 452 I0NFO2
// MATCHING 800eb550 452
#include "TOBJ.H"
typedef struct { unsigned short v; } SpW;
#define SPAD16(a) (((SpW *)(0x1F800000 + (a)))->v)
extern void func_80025D90(signed char *p);
extern void func_8005BD88(void *dst, void *src, int n);
extern void topdownPlayerInit(TObj *o);
extern void topdownPlayerMainStep(TObj *o);
extern void topdownPlayerEventStep(TObj *o);
extern unsigned short D_8007A034, D_8007A036;
extern signed char D_8009D2B0[];
extern unsigned short D_8009D610;
extern unsigned short D_8009D670[];
extern unsigned char D_8009F7F0;
extern unsigned short D_800A6040, D_800A60FC;

void topdownPlayerHandler(TObj *o)
{
    unsigned short pad[4];
    unsigned short b0, b1;
    signed char *m;
    func_8005BD88(pad, D_8009D670, 8);
    b0 = *(unsigned short *)0x1F8001FC;
    b1 = *(unsigned short *)0x1F8001FE;
    if (o->subtype == 1) {
        D_800A6040 = D_8007A034;
        D_800A60FC = D_8007A036;
    }
    if (o->subtype == 2) {
        D_800A6040 = D_8007A034;
        D_800A60FC = D_8007A036;
    }
    m = D_8009D2B0;
    if (*m >= 5) goto clr;
    if (*m < 3) {
    clr:
        *m = 0;
    }
    if (D_8009F7F0 == 0 && D_8009D610 == 7) func_80025D90(m);
    switch (o->b04) {
    case 0:
        topdownPlayerInit(o);
        break;
    case 1:
        topdownPlayerMainStep(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        break;
    case 4:
        topdownPlayerEventStep(o);
        break;
    }
    SPAD16(0x16A) = o->h->p.whole;
    SPAD16(0x16E) = o->y.p.whole;
    SPAD16(0x172) = o->d->p.whole;
    func_8005BD88(D_8009D670, pad, 8);
    *(unsigned short *)0x1F8001FC = b0;
    *(unsigned short *)0x1F8001FE = b1;
}
