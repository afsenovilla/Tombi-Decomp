// FUNC 800ea948 248 D0SPSUB2
// MATCHING 800ea948 248
typedef struct L { short x0, x1, x2, x3; short a, b, c, d; short m; } L;
extern char D_800F6B58[], D_800F6C18[], D_800F6C38[];
extern short D_800F70DC;
extern void func_800EAA40(char *s, int x, int y);
extern void func_800EABA4(short x, short y);
extern void AddDrawMode(int a, int b);
extern void drawBalloonFrame(L *, int, int);

void func_800EA948(int yes, int no, int sel)
{
    L l;
    if (sel != 0) {
        func_800EAA40(D_800F6B58, 0x74, 0x70);
        if (yes) func_800EAA40(D_800F6C18, 0x68, 0x88);
        if (no) func_800EAA40(D_800F6C38, 0xA8, 0x88);
        if (sel == 1) func_800EABA4(0x68, 0x80);
        else if (sel == 2) func_800EABA4(0xA8, 0x80);
        D_800F70DC = 1;
        AddDrawMode(1, 1);
        l.m = -1;
        l.a = 0x60;
        l.b = 0x60;
        l.c = 0x80;
        l.d = 0x38;
        drawBalloonFrame(&l, 0, 0x14);
    }
}
