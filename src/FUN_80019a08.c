// FUNC 80019a08 260 MAIN0
// MATCHING 80019a08 260
typedef struct { short x, y, w, h; } RECT;
extern char *DAT_1f8001d4;
extern unsigned char DAT_1f8001cf;
extern void ClearImage(RECT *r, int a, int b, int c);
extern void FUN_8001783c(void);

void FUN_80019a08(void)
{
    char *s = DAT_1f8001d4;
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
        FUN_8001783c();
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
