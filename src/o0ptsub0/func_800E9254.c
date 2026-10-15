// FUNC 800e9254 696 O0PTSUB0
// MATCHING 800e9254 696
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef D_800EB20C[];
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void func_800E8C58(SprDef *d, short tpage, unsigned char pri);
extern void func_800E9798(short x, short y, int i);
extern void func_800E97F4(short x, short y, int i);

void func_800E9254(unsigned int sel)
{
    SprDef *d;
    short tp;
    switch (sel) {
    case 0:
        func_800E97F4(0x20, 0x54, 0x18);
        func_800E97F4(0xE4, 0x3C, 0x19);
        func_800E97F4(0xF4, 0x5C, 0x1A);
        func_800E97F4(0xE4, 0x9C, 0x1B);
        func_800E97F4(0x54, 0x84, 0x1C);
        break;
    case 1:
        func_800E9798(0x38, 0x50, 0xF);
        func_800E97F4(0xE4, 0x3C, 0x1B);
        func_800E97F4(0xF4, 0x5C, 0x19);
        func_800E97F4(0xE4, 0x9C, 0x1C);
        func_800E97F4(0x54, 0x84, 0x1A);
        break;
    case 2:
        func_800E9798(0x38, 0x50, 0x10);
        func_800E97F4(0xE4, 0x3C, 0x19);
        func_800E97F4(0xF4, 0x5C, 0x1C);
        func_800E97F4(0xE4, 0x9C, 0x1B);
        func_800E97F4(0x54, 0x84, 0x1A);
        break;
    case 3:
        func_800E9798(0x38, 0x50, 0x11);
        func_800E97F4(0xE4, 0x3C, 0x19);
        func_800E97F4(0xF4, 0x5C, 0x1A);
        func_800E97F4(0xE4, 0x9C, 0x1C);
        func_800E97F4(0x54, 0x84, 0x1B);
        break;
    case 4:
        func_800E9798(0x38, 0x50, 0x12);
        func_800E97F4(0xE4, 0x3C, 0x1B);
        func_800E97F4(0xF4, 0x5C, 0x1C);
        func_800E97F4(0xE4, 0x9C, 0x1A);
        func_800E97F4(0x54, 0x84, 0x19);
        break;
    case 5:
        func_800E9798(0x38, 0x50, 0x13);
        func_800E97F4(0xE4, 0x3C, 0x1C);
        func_800E97F4(0xF4, 0x5C, 0x1B);
        func_800E97F4(0xE4, 0x9C, 0x1A);
        func_800E97F4(0x54, 0x84, 0x19);
        break;
    }
    tp = GetTPage(0, 0, 0x200, 0);
    d = D_800EB20C;
    while (d->x != -1) {
        func_800E8C58(d++, tp, 0);
    }
    func_800E9798(0x74, 0x28, 5);
    func_800E97F4(0x60, 0xC0, 0x14);
    func_800E97F4(0x60, 0xD0, 0x16);
    func_800E97F4(0x20, 0x40, 0x17);
}
