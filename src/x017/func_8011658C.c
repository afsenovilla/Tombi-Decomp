// FUNC 8011658c 916 X017
// MATCHING 8011658c 916
/* Covers splat entries func_8011658C (prologue) + func_80117CBC (body). */
#include "TOBJ.H"

typedef struct { char pad[0xc6]; unsigned short wc6; } XC;
typedef struct { short s[10]; } E;
typedef struct { E e[6]; char pad[0x8c - 0x78]; } G;
typedef struct { short s0, s2, s4, s6, s8, sa; char pad[0x8c - 0xc]; } G2;
typedef struct { short a, b, c, d, e, f, g, h, i; } T;

extern G D_800A3FE0[];
extern G2 D_800A4058[];
extern T D_801197FA[];
extern short *D_80119974[];
extern int ObjCullRegister(TObj *);
extern void FUN_800187e4(TObj *);

#define g ((G2 *)e)

void func_8011658C(TObj *o)
{
    E *e;
    short *p;
    int i;

    switch (o->b04) {
    case 0:
        if (--o->timer == 0) o->b04++;
        break;
    case 1:
        if (ObjCullRegister(o) == 0) o->b04++;
        for (i = 0; i < 6; i++) {
            e = &D_800A3FE0[((XC *)o)->wc6].e[i];
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
                    p = &D_801197FA[e->s[0]].a;
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
        e = (E *)&D_800A4058[((XC *)o)->wc6];
        switch (g->s0) {
        case 0:
            break;
        case 1:
            if (--g->s2 > 0) break;
            o->d90 = (int)D_80119974[o->subtype];
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
        D_800A3FE0[((XC *)o)->wc6].e[0].s[0] = -1;
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
