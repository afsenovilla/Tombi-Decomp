// FUNC 8010e328 700 X018
// MATCHING 8010e328 700
#include "TOBJ.H"
typedef struct {
    TObj o;
    unsigned char bc0, bc1, bc2, bc3;
    char pc4[0xcc - 0xc4];
    unsigned char bcc, bcd, bce;
    char pcf[0xe3 - 0xcf];
    signed char be3;
} S;
typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6, b7; } P;
extern unsigned short DAT_1f8003c8;
extern unsigned short DAT_1f8001fc;
extern unsigned char D_8009D2B0;
extern unsigned char D_8009C990;
extern unsigned char D_8009D2B1;
extern unsigned char D_8009D2B2;
extern short D_8007A038[];
extern P *D_8009C330;
extern TObj *D_8009F0EC;
extern void func_80030034(int);
extern short func_8010E114(void);

void func_8010E328(S *s, short mode)
{
    P *p;
    P *q;
    short v, t;

    if ((DAT_1f8001fc & DAT_1f8003c8) == 0) return;
    D_8009D2B0 = 0;
    s->bcd = 0;
    s->bce = 0;
    switch (D_8009C990) {
    case 1:
        if (s->bcc == 1) func_80030034(0);
        func_8010E114();
        return;
    case 2:
        if (s->bcc == 1) func_80030034(1);
        func_8010E114();
        return;
    }
    s->bc3 = 0;
    if (s->be3 >= D_8007A038[D_8009D2B2]) return;
    switch (mode) {
    case 0:
        switch (D_8009D2B1) {
        case 1:
            s->o.step = 0x2a;
            s->o.state = 0;
            return;
        case 2:
            s->o.step = 0x2b;
            s->o.state = 0;
            return;
        }
        if (func_8010E114() != 0) return;
        if (D_8009C330->b0 != 0) return;
        if (s->bc0 != 0) return;
        s->o.step = 3;
        s->o.state = 0;
        return;
    case 1:
        if (D_8009D2B1 == 1 || D_8009D2B1 == 2) return;
        p = D_8009C330;
        if (p->b0 != 0) return;
        if (s->bc0 != 0) return;
        if (*(unsigned char *)&s->o.wac == 2) return;
        p->b7 = s->o.animFrame;
        v = s->o.wb2;
        if (v < 0) {
            t = v;
            if (!(s->o.animFrame & 1))
                t = -v;
            s->o.wb2 = t;
        } else {
            if (s->o.animFrame & 1)
                v = -v;
            s->o.wb2 = v;
        }
        s->o.step = 4;
        s->o.state = 0;
        return;
    case 2:
        if (D_8009D2B1 == 1 || D_8009D2B1 == 2) return;
        if (D_8009C330->b0 != 0) return;
        if (s->bc0 != 0) return;
        D_8009C330->b4 = 0;
        D_8009F0EC->b69 = 4;
        s->o.velH = 0;
        s->o.velV = 0;
        s->o.velX = 0;
        s->o.velY = 0;
        s->o.step = 0x18;
        s->o.state = 0;
        break;
    }
}
