// VERSION BETA_980203 beta980203 8001A150 220
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/gameInitState.c: no VRAM ClearImage, two init calls (func_80017408, func_80028D30) in place of initGameConfig.
typedef struct { short x, y, w, h; } RECT;
extern char *TASK_C;
extern unsigned char DAT_1f8001cf;
extern void ClearImage(RECT *r, int a, int b, int c);
extern void func_80017408(void);
extern void func_80028D30_beta980203(void);

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
        func_80017408();
        func_80028D30_beta980203();
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
