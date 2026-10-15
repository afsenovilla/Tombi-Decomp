// FUNC 800e90d0 388 O0PTSUB0
// MATCHING 800e90d0 388
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef D_800EB1DC[];
extern unsigned short D_8009D612;
extern short D_800EB480;
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void func_800E8C58(SprDef *d, short tpage, unsigned char pri);
extern void func_800E9798(short x, short y, int i);
extern void func_800E97F4(short x, short y, int i);

void func_800E90D0(int sound, int vib)
{
    SprDef *d;
    short tp;
    tp = GetTPage(0, 0, 0x200, 0);
    d = D_800EB1DC;
    while (d->x != -1) {
        func_800E8C58(d++, tp, 0);
    }
    func_800E9798(0x88, 0x1E, 0);
    func_800E9798(0x50, 0x3C, 1);
    func_800E9798(0xB0, 0x3C, sound + 6);
    func_800E9798(0x50, 0x54, 2);
    func_800E9798(0xB0, 0x54, vib + 9);
    func_800E97F4(0x60, 0xC0, 0x14);
    func_800E97F4(0x60, 0xD0, 0x16);
    if (D_800EB480 != 0) {
        func_800E9798(0x50, 0x6C, 3);
        func_800E9798(0xB0, 0x6C, D_8009D612 + 0xB);
        func_800E9798(0x50, 0x84, 4);
        func_800E9798(0x50, 0x9C, 5);
    } else {
        func_800E9798(0x50, 0x6C, 4);
        func_800E9798(0x50, 0x84, 5);
    }
}
