// FUNC 800eb50c 416 X008
// MATCHING 800eb50c 416
typedef struct { char p[0x1e]; unsigned short w1e; } NN;
typedef struct { unsigned short w; short idx; void **ptrs; int pad; } E;
extern E DAT_80114ae4[];
extern unsigned short DAT_8009c960[];
extern int DAT_1f8002c8[];
extern unsigned char DAT_800a6047;
extern char *DAT_800a611c;
extern char *FUN_80018448(void);
extern void FUN_8001fe6c(char *);

void FUN_800eb50c(short a, int x, int y, int z)
{
    char *n;
    n = FUN_80018448();
    if (n != 0) {
        n[0] = 1;
        n[2] = 0x1b;
        *(int *)(n + 0x10) = x << 16;
        *(int *)(n + 0x14) = y << 16;
        n[3] = a;
        *(int *)(n + 0x18) = z << 16;
        ((NN *)n)->w1e = DAT_80114ae4[DAT_8009c960[0]].w;
        *(int *)(n + 0x3c) = DAT_1f8002c8[DAT_80114ae4[DAT_8009c960[0]].idx];
        if (a == 2 && *(int *)DAT_8009c960 == 0x50000) a = 3;
        *(void **)(n + 0x24) = DAT_80114ae4[DAT_8009c960[0]].ptrs[a];
        FUN_8001fe6c(n);
        n[0xd] = 0;
        n[0xa] = 2;
        n[0x68] = 0;
        n[0x1c] |= 0x80;
        n[0xf] = DAT_800a6047 - 1;
        if (a == 0) DAT_800a611c = n;
    }
}
