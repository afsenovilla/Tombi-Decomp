// FUNC 80116900 272 X014
// MATCHING 80116900 272

typedef struct { short x, y, w, h; } RECT;
extern int LoadImage(RECT *, void *);
extern char D_801256B8[], D_801256F8[], D_80125738[], D_80125778[];

void func_80116900(short n, short i)
{
    RECT r;

    switch (n) {
    case 0:
        r.x = 0x130;
        r.y = 0x1e0;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_801256B8 + (i << 5));
        break;
    case 1:
        r.x = 0x130;
        r.y = 0x1e1;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_801256F8 + (i << 5));
        break;
    case 2:
        r.x = 0x130;
        r.y = 0x1e2;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_80125738 + (i << 5));
        break;
    case 3:
        r.x = 0x130;
        r.y = 0x1e6;
        r.w = 0x10;
        r.h = 1;
        LoadImage(&r, D_80125778 + (i << 5));
        break;
    }
}
