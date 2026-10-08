// FUNC 80111b40 648 X010
// MATCHING 80111b40 648
typedef struct { short lo, hi; } HL;
typedef union { int i; HL s; } FX;
typedef struct { FX x, y, z; } V3;
typedef struct { char p0[2]; short s2; } H;
typedef struct { short a, b; } AB;
typedef struct O {
    char p0[0x16]; unsigned short w16; char p1[0x40 - 0x18]; H *h, *h44; char p2[0x6a - 0x48]; unsigned char b6a;
} O;
extern unsigned char T_80115a58[][6];
extern unsigned char *L_80115d3c[];
extern AB D_80115d70[];
extern unsigned char DAT_8009ce4a, DAT_8009d0b8;
extern int FUN_8001f9e0(void);
extern void FUN_8003ecb0(unsigned char *, V3 *, int, int, O *);

void FUN_80111b40(O *o)
{
    V3 v;
    unsigned char *e, *l;
    int i;
    v.x.s.hi = o->h->s2;
    v.y.s.hi = o->w16;
    v.z.s.hi = o->h44->s2;
    e = T_80115a58[o->b6a];
    if (e[0] == 0xff) {
        l = L_80115d3c[e[1]];
        for (i = 0; *l != 0xff; i++) {
            FUN_8003ecb0(l, &v, D_80115d70[i].a, D_80115d70[i].b, o);
            l += 6;
        }
    } else if (e[0] == 0xfe) {
        switch (e[1]) {
        case 0:
            if ((FUN_8001f9e0() & 7) < 4) {
                FUN_8003ecb0(L_80115d3c[1], &v, 0, -0x400, o);
            } else {
                l = L_80115d3c[0];
                for (i = 0; *l != 0xff; i++) {
                    FUN_8003ecb0(l, &v, D_80115d70[i].a, D_80115d70[i].b, o);
                    l += 6;
                }
            }
            break;
        case 1:
            if (DAT_8009ce4a == 0xff) FUN_8003ecb0(L_80115d3c[8], &v, 0, -0x400, o);
            else FUN_8003ecb0(L_80115d3c[7], &v, 0, -0x400, o);
            break;
        case 2:
            if (DAT_8009d0b8 == 0) FUN_8003ecb0(L_80115d3c[9], &v, 0, -0x400, o);
            else FUN_8003ecb0(L_80115d3c[10], &v, 0, -0x400, o);
            break;
        }
    } else {
        FUN_8003ecb0(e, &v, 0, -0x400, o);
    }
}
