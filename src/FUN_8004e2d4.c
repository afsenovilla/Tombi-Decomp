// FUNC 8004e2d4 328 MAIN0
// MATCHING 8004e2d4 328
typedef struct Th { char pad[0x48]; short a; unsigned short c; } Th;
typedef struct { short x, y, w, h; } RECT;
extern Th *DAT_1f8001d4;
extern char DAT_1f8001cc, DAT_1f8001cd;
extern void SetDispMask(int);
extern void FUN_80016ca8(int, int, int);
extern void ClearImage(RECT *, int, int, int);
extern void ThreadCreate(int, void *);
extern void MusicStopOrFadeIn(int);
extern char LAB_8001d6a4[];

void FUN_8004e2d4(void)
{
    Th *t = DAT_1f8001d4;
    unsigned short c = t->c;
    RECT r;
    Th *u;
    switch (c) {
    case 0:
        SetDispMask(0);
        FUN_80016ca8(0, 0, 0);
        u = DAT_1f8001d4;
        u->c = u->c + 1;
        r.x = 0x180;
        r.y = 0x100;
        r.w = 0x280;
        r.h = 0x100;
        ClearImage(&r, 0, 0, 0);
        break;
    case 1:
        DAT_1f8001cc = 1;
        DAT_1f8001cd = 0;
        ThreadCreate(1, LAB_8001d6a4);
        u = DAT_1f8001d4;
        u->c = u->c + 1;
        break;
    case 2:
        if (DAT_1f8001cc == 0) t->c = c + 1;
        break;
    case 3:
        MusicStopOrFadeIn(0);
        u = DAT_1f8001d4;
        u->a = 4;
        u->c = 0;
        break;
    }
}
