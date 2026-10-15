// FUNC 800e96a4 136 O0PTSUB0
// MATCHING 800e96a4 136
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef D_800EB3EC[];
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void func_800E8C58(SprDef *d, short tpage, unsigned char pri);

void func_800E96A4(void)
{
    SprDef *d;
    short tp;
    tp = GetTPage(1, 0, 0x180, 0);
    d = D_800EB3EC;
    while (d->x != -1) {
        func_800E8C58(d++, tp, 4);
    }
}
