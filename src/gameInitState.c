// FUNC 80019a08 260 MAIN0
// MATCHING 80019a08 260
// VERSION SLPM_80165 slpm80165 8001976C 220
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80019A08 (gameInitState.c): calls func_8001653C instead of clearing VRAM.
typedef struct { short x, y, w, h; } RECT;
extern char *TASK_C;
extern unsigned char DAT_1f8001cf;
extern void ClearImage(RECT *r, int a, int b, int c);
extern void initGameConfig(void);
extern void func_8001653C(void);

void gameInitState(void)
{
    char *s = TASK_C;
    unsigned short u = *(unsigned short *)(s + 0x4a);
    short *q;
    short v;
    switch (u) {
    case 0:
        q = (short *)(s + 0x4a);
        *(short *)(s + 0x5a) = 1;
        *(short *)(s + 0x4a) = *q + 1;
        func_8001653C();
        initGameConfig();
        DAT_1f8001cf = 0;
        break;
    case 1:
        u = *(short *)(s + 0x5a) - 1;
        *(unsigned short *)(s + 0x5a) = u;
        if ((int)((unsigned)u << 16) < 1)
            *(short *)(s + 0x4a) = *(short *)(s + 0x4a) + 1;
        break;
    case 2:
        {
        char c = *(char *)(s + 0x68);
        *(short *)(s + 0x4c) = 0;
        *(short *)(s + 0x4e) = 0;
        if (c) *(short *)(s + 0x48) = 2; else *(short *)(s + 0x48) = 1;
        *(short *)(s + 0x4a) = 0;
        }
    }
}
#else
typedef struct { short x, y, w, h; } RECT;
extern char *TASK_C;
extern unsigned char DAT_1f8001cf;
extern void ClearImage(RECT *r, int a, int b, int c);
extern void initGameConfig(void);

void gameInitState(void)
{
    char *s = TASK_C;
    RECT r;
    unsigned short u = *(unsigned short *)(s + 0x4a);
    short *q;
    short v;
    switch (u) {
    case 0:
        q = (short *)(s + 0x4a);
        *(short *)(s + 0x5a) = 1;
        *(short *)(s + 0x4a) = *q + 1;
        r.w = 0x40;
        r.x = 0;
        r.y = 0;
        r.h = 0x100;
        ClearImage(&r, 0, 0, 0);
        initGameConfig();
        DAT_1f8001cf = 0;
        break;
    case 1:
        u = *(short *)(s + 0x5a) - 1;
        *(unsigned short *)(s + 0x5a) = u;
        if ((int)((unsigned)u << 16) < 1)
            *(short *)(s + 0x4a) = *(short *)(s + 0x4a) + 1;
        break;
    case 2:
        {
        char c = *(char *)(s + 0x68);
        *(short *)(s + 0x4c) = 0;
        *(short *)(s + 0x4e) = 0;
        if (c) *(short *)(s + 0x48) = 2; else *(short *)(s + 0x48) = 1;
        *(short *)(s + 0x4a) = 0;
        }
    }
}
#endif
