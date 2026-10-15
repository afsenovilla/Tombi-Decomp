// FUNC 800ed380 336 I0NFO2
// MATCHING 800ed380 336
extern void func_800ECC18(int i, int p);
extern void func_800174C0(int p, int x, int y, int w, int h);
extern unsigned short D_8009C960[];

void func_800ED380(unsigned char i, int p)
{
    int x, y, w, h;
    switch (i) {
    case 0:
        x = 0;
        y = 0;
        w = 0x80;
        h = 0x100;
        break;
    case 1:
        x = 0x80;
        y = 0;
        w = 0x40;
        h = 0x100;
        break;
    case 2:
        x = 0x40;
        y = 0;
        w = 0x40;
        h = 0x20;
        break;
    case 3:
        x = 0x40;
        y = 0x20;
        w = 0x40;
        h = 0x20;
        break;
    case 4:
        x = 0x40;
        y = 0x40;
        w = 0x40;
        h = 0x20;
        break;
    }
    func_800ECC18(i, p);
    if (i < 2) {
        func_800174C0(p, x + 0x300, y, w, h);
    } else if (D_8009C960[0] == 5) {
        func_800174C0(p, 0x40, y + 0x180, w, h);
    } else if (*(int *)D_8009C960 == 0x20013) {
        func_800174C0(p, 0x40, y, w, h);
    } else {
        func_800174C0(p, 0, y + 0x100, w, h);
    }
}
