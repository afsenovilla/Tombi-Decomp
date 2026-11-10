// VERSION PCPX_96084 tomba 8002FEF0 900
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x017 8011658C (updatePointsBalloon.c): slot index at +0xAA (PAL +0xC6), state 1 stops when the cull call fails.
/* Covers splat entries func_8002FEF0_pcpx96084 (prologue) + func_80117CBC (body). */
#include "TOBJ.H"

typedef struct { char pad[0xaa]; unsigned short wc6; } XC;
typedef struct { short s[10]; } E;
typedef struct { E e[6]; char pad[0x8c - 0x78]; } G;
typedef struct { short s0, s2, s4, s6, s8, sa; char pad[0x8c - 0xc]; } G2;
typedef struct { short a, b, c, d, e, f, g, h, i; } T;

extern G D_800E622C[];
extern G2 D_800E62A4[];
extern T D_800A27CE[];
extern short *D_800A2948[];
extern int func_80018550(TObj *);
extern void func_8001A87C_pcpx96084(TObj *);

#define g ((G2 *)e)

void func_8002FEF0_pcpx96084(TObj *o)
{
    E *e;
    short *p;
    int i;

    switch (o->b04) {
    case 0:
        if (--o->timer == 0) o->b04++;
        break;
    case 1:
        if (func_80018550(o) == 0) break;
        for (i = 0; i < 6; i++) {
            e = &D_800E622C[((XC *)o)->wc6].e[i];
            switch (e->s[0]) {
            case 0:
            case 6:
                break;
            case 2:
            case 3:
                if (e->s[6] <= 0) e->s[7] = 0;
                if (e->s[8] <= 0) e->s[9] = 0;
            case 1:
            case 4:
            case 5:
                e->s[3] -= e->s[6];
                e->s[5] -= e->s[8];
                e->s[6] += e->s[7];
                e->s[8] += e->s[9];
                if (--e->s[1] <= 0) {
                    p = &D_800A27CE[e->s[0]].a;
                    e->s[1] = *p++;
                    e->s[6] = *p++;
                    e->s[7] = *p++;
                    e->s[8] = p[0];
                    e->s[9] = p[1];
                    e->s[0]++;
                }
                break;
            }
        }
        e = (E *)&D_800E62A4[((XC *)o)->wc6];
        switch (g->s0) {
        case 0:
            break;
        case 1:
            if (--g->s2 > 0) break;
            o->d90 = (int)D_800A2948[o->subtype];
            g->s2 = 0x12;
            g->s0++;
            break;
        case 2:
            p = (short *)o->d90;
            g->s4 += p[0];
            g->s6 += p[1];
            g->s8 -= p[0];
            g->sa -= p[1];
            o->d90 = (int)(p + 2);
            if (--g->s2 > 0) break;
            g->s2 = 0x1e;
            g->s0++;
            break;
        case 3:
            if (--g->s2 > 0) break;
            o->b04 = 2;
            break;
        }
        break;
    case 2:
        D_800E622C[((XC *)o)->wc6].e[0].s[0] = -1;
        o->b04++;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
