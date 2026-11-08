// VERSION SLPS_01144 dspsub 800EC7B8 544
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800eb148 (subScreenDrawGaugePercent.c): three digits
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; unsigned short attr; int img; } Text;
extern Slot D_80109EEC[];
extern Text D_80111408;
extern unsigned char D_8009E944[];
extern void func_800EC9D8(int n);

void func_800EC7B8(Menu *m, int i)
{
    Slot *s = &D_80109EEC[i];
    int pct;
    int dx;
    D_80111408.x = s->x;
    D_80111408.y = s->y;
    D_80111408.w = m->w10 + s->dw;
    D_80111408.attr = 0x7C15;
    D_80111408.img = m->p58[s->idx];
    func_800EC9D8(1);
    pct = (60 - D_8009E944[i]) * 100 / 60;
    dx = 0;
    if (pct == 100) {
        D_80111408.x = s->x + 30;
        D_80111408.y = s->y;
        D_80111408.img = m->p5c[1];
        func_800EC9D8(1);
        dx = 8;
    }
    if (pct >= 10) {
        { int o = dx + 30; D_80111408.x = s->x + o; }
        D_80111408.y = s->y;
        D_80111408.img = m->p5c[pct / 10 % 10];
        func_800EC9D8(1);
    }
    { int o = dx + 40; D_80111408.x = s->x + o; }
    D_80111408.y = s->y;
    D_80111408.img = m->p5c[pct % 10];
    func_800EC9D8(1);
}
#endif
