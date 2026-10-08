// FUNC 8001aeb4 1140 MAIN0
// MATCHING 8001aeb4 1140
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    RECT clip; short ofs[2]; RECT tw; unsigned short tpage;
    unsigned char dtd, dfe, isbg, r0, g0, b0;
    unsigned long dr_env[16];
} DRAWENV;
typedef struct {
    char p0[0x48];
    short w48, w4a, w4c;
    unsigned short w4e;
    char p50[0x68 - 0x50];
    unsigned char b68, b69, b6a;
} G;
extern G *D_1F8001D4;
extern unsigned char D_1F8001CF, D_1F8001CE, D_1F8001C2;
extern short D_1F8001C6;
extern unsigned short D_1F8003B8, D_1F8003BA;
extern unsigned char D_8009E375[], D_8009E376[], D_8009E377[], D_8009F085[], D_8009F086[], D_8009F087[];
extern unsigned char D_8009BC98, D_8009BC9C, D_8009BCA0;
extern short D_800A45EA;
extern unsigned short D_800A45EAu;
extern unsigned short D_8009D670;
extern unsigned short D_8009C960, D_8009F838, D_8009C962;
extern short *D_80077A8C[];
extern unsigned char D_8009C974, D_8009C967;
extern void SetDispMask(int);
extern int StoreImage(RECT *, unsigned long *);
extern int LoadImage(RECT *, unsigned long *);
extern DRAWENV *GetDrawEnv(DRAWENV *);
extern void EnterCriticalSection(void);
extern void FlushCache(void);
extern void ExitCriticalSection(void);
extern void FUN_80018280(void);
extern void FUN_8001b328(void);
extern void FUN_8004fa80(int, int);
extern void stopBgm(int);

void func_8001AEB4(void)
{
    DRAWENV env;
    RECT r;
    int n;
    unsigned short s;
    volatile unsigned short *k;

    switch (D_1F8001D4->w4e) {
    case 0:
        D_1F8001CF = 1;
        D_1F8001D4->b6a = 0;
        SetDispMask(0);
        r.x = 0x120;
        r.y = 0x1e0;
        r.w = 0x30;
        r.h = 0x1f;
        StoreImage(&r, (unsigned long *)0x801FB000);
        r.x = 0x80;
        r.y = 0x1ff;
        r.w = 0x100;
        r.h = 1;
        StoreImage(&r, (unsigned long *)0x801FBBA0);
        GetDrawEnv(&env);
        D_8009F085[0] = 0xf8;
        D_8009E376[0] = 200;
        D_8009F087[0] = 0xc0;
        D_8009E377[0] = 0xc0;
        D_8009F086[0] = 200;
        D_8009BC98 = env.r0;
        D_8009BC9C = env.g0;
        D_8009BCA0 = env.b0;
        D_8009E375[0] = 0xf8;
        D_1F8001D4->w4e++;
        break;
    case 1:
        D_1F8001CE = 0;
        switch (D_800A45EA) {
        case 0: case 1: case 2: case 6: n = 3; break;
        case 4: n = 4; break;
        case 5: n = 5; break;
        case 7: n = 6; break;
        case 3: n = 7; break;
        default: goto next;
        }
        goto play;
    case 2:
        if (D_1F8001CE == 0) break;
        EnterCriticalSection();
        FlushCache();
        ExitCriticalSection();
        {
            unsigned short *q = &D_800A45EAu;
            s = *q;
            FUN_80018280();
            *q = s;
        }
        SetDispMask(1);
        goto next;
    case 3:
        FUN_8001b328();
        if (D_1F8001C2 != 0) {
            k = &D_8009D670;
            if ((*k & 8) && (*k & 0x800)) D_1F8001D4->w4e = 6;
        }
        break;
    case 4:
        SetDispMask(0);
        r.x = 0x120;
        r.y = 0x1e0;
        r.w = 0x30;
        r.h = 0x1f;
        LoadImage(&r, (unsigned long *)0x801FB000);
        r.x = 0x80;
        r.y = 0x1ff;
        r.w = 0x100;
        r.h = 1;
        LoadImage(&r, (unsigned long *)0x801FBBA0);
        D_1F8001CE = 0;
        D_8009E375[0] = D_8009BC98;
        D_8009F085[0] = D_8009BC98;
        D_8009E376[0] = D_8009BC9C;
        D_8009E377[0] = D_8009BCA0;
        D_8009F086[0] = D_8009BC9C;
        D_8009F087[0] = D_8009BCA0;
        n = D_80077A8C[D_8009C960 + D_8009F838][D_8009C962];
    play:
        FUN_8004fa80(n, 1);
    next:
        D_1F8001D4->w4e++;
        break;
    case 5:
        if (D_1F8001CE != 0) {
            EnterCriticalSection();
            FlushCache();
            ExitCriticalSection();
            SetDispMask(1);
            D_1F8001C6 = 0;
            D_1F8001D4->w4c = D_1F8003B8;
            D_1F8001D4->w4e = D_1F8003BA;
        }
        break;
    case 6:
        D_1F8001D4->w4c = 8;
        D_1F8001D4->w4e = 0;
        break;
    case 7:
        D_8009C974 = 0;
        D_8009C967 = 0;
        stopBgm(0);
        D_8009E375[0] = 0;
        D_8009E376[0] = 0;
        D_8009E377[0] = 0;
        D_8009F085[0] = 0;
        D_8009F086[0] = 0;
        D_8009F087[0] = 0;
        D_1F8001D4->b68 = 1;
        D_1F8001D4->w48 = 1;
        D_1F8001D4->w4a = 1;
        D_1F8001D4->w4c = 0;
        D_1F8001D4->w4e = 0;
        break;
    }
}
