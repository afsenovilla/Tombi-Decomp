// FUNC 800eb0a0 428 D0SPSUB3
// MATCHING 800eb0a0 428
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; unsigned short attr; int img; } Text;
extern Slot D_801007A4[];
extern Text D_80100B70;
extern unsigned char D_8009CD9C[];
extern void func_800EB24C(int n);

void func_800EB0A0(Menu *m, int i)
{
    Slot *s = &D_801007A4[i];
    int pct;
    int dx;
    D_80100B70.x = s->x;
    D_80100B70.y = s->y;
    D_80100B70.w = m->w10 + s->dw;
    D_80100B70.attr = 0x7C15;
    D_80100B70.img = m->p58[s->idx];
    func_800EB24C(1);
    pct = (60 - D_8009CD9C[i]) * 100 / 60;
    dx = 0;
    if (pct == 100) {
        D_80100B70.x = s->x + 0x26;
        D_80100B70.y = s->y;
        D_80100B70.img = m->p5c[1];
        func_800EB24C(1);
        dx = 8;
    }
    { int o = dx + 0x26; D_80100B70.x = s->x + o; }
    D_80100B70.y = s->y;
    D_80100B70.img = m->p5c[pct / 10 % 10];
    func_800EB24C(1);
}
