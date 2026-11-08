// VERSION SLPS_01144 dspsub3 800EC700 544
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800eb148 (dspsub3 copy; subScreenDrawGaugePercent.c): three digits
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; unsigned short attr; int img; } Text;
extern Slot D_800FF19C[];
extern Text D_800FF57C;
extern unsigned char D_8009E944[];
extern void func_800EC920(int n);

void func_800EC700(Menu *m, int i)
{
    Slot *s = &D_800FF19C[i];
    int pct;
    int dx;
    D_800FF57C.x = s->x;
    D_800FF57C.y = s->y;
    D_800FF57C.w = m->w10 + s->dw;
    D_800FF57C.attr = 0x7C15;
    D_800FF57C.img = m->p58[s->idx];
    func_800EC920(1);
    pct = (60 - D_8009E944[i]) * 100 / 60;
    dx = 0;
    if (pct == 100) {
        D_800FF57C.x = s->x + 30;
        D_800FF57C.y = s->y;
        D_800FF57C.img = m->p5c[1];
        func_800EC920(1);
        dx = 8;
    }
    if (pct >= 10) {
        { int o = dx + 30; D_800FF57C.x = s->x + o; }
        D_800FF57C.y = s->y;
        D_800FF57C.img = m->p5c[pct / 10 % 10];
        func_800EC920(1);
    }
    { int o = dx + 40; D_800FF57C.x = s->x + o; }
    D_800FF57C.y = s->y;
    D_800FF57C.img = m->p5c[pct % 10];
    func_800EC920(1);
}
#endif
