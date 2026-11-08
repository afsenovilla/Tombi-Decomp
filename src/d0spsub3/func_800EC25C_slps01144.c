// VERSION SLPS_01144 dspsub3 800EC25C 644
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800EAC9C (subScreenDrawItemNames.c), from the USA one: 16 items per page
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[8]; short sel; char p20[0x3C]; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; int pC; int str; void *p14; } Text;
extern Slot D_800FE144[];
extern Text D_800FF57C;
extern unsigned short D_8009EE4C;
extern unsigned char D_800FF394[];
extern unsigned char D_800FEC30[];
extern int D_800FECF8[];
extern unsigned char D_800FF054[];
extern unsigned char D_8009EC4C[];
extern char D_800FAABC[];
extern void func_800ED024(int pri);
extern void func_800EC920(int n);

void func_800EC25C(Menu *m)
{
    Slot *s;
    for (s = D_800FE144; s->x != -1; s++) {
        int i;
        int n;
        D_800FF57C.x = s->x + 0x18;
        D_800FF57C.y = s->y + 5;
        D_800FF57C.w = m->w10 + 1;
        i = m->page * 16 + s->idx;
        if (i >= D_8009EE4C) continue;
        D_800FF57C.str = D_800FECF8[D_800FEC30[D_800FF394[i]]];
        D_800FF57C.p14 = D_800FAABC;
        if (i == m->sel) D_800FF57C.attr = 0x7C15;
        else D_800FF57C.attr = (((D_800FF054[D_800FF394[i]] & 0x7F) + 0x1F0) << 6) | 0x15;
        func_800ED024(5);
        n = D_8009EC4C[D_800FF394[i]];
        if (D_800FF054[D_800FF394[i]] & 0x80) {
            D_800FF57C.x += 0x58;
            D_800FF57C.y += 0xA;
            D_800FF57C.w = m->w10;
            D_800FF57C.img = m->p5c[10];
            func_800EC920(1);
            if (n >= 10) {
                D_800FF57C.x += 0xA;
                D_800FF57C.img = m->p5c[n / 10 % 10];
                func_800EC920(1);
            }
            D_800FF57C.x += 0xA;
            D_800FF57C.img = m->p5c[n % 10];
            func_800EC920(1);
        }
    }
}
#endif
