// FUNC 80030b9c 268 MAIN0
extern char *FUN_800184d8(void);
extern unsigned short DAT_800a6066;
extern unsigned char uRam8009cef7;
extern int DAT_8009c984;
extern void FUN_8004d620(int, int);

void FUN_80030b9c(short a, int x, int y, int z)
{
    char *p = FUN_800184d8();
    unsigned short u;
    if (p) {
        *p = 1;
        p[2] = 0x20;
        u = DAT_800a6066;
        *(int *)(p + 0x10) = x << 16;
        *(int *)(p + 0x14) = y << 16;
        *(int *)(p + 0x18) = z << 16;
        p[3] = a;
        p[0xd] = 0;
        *(unsigned short *)(p + 0x2e) = u & 1;
        if (a == 0) {
            p[5] = 1;
        } else {
            p[5] = a;
            uRam8009cef7 = 1;
        }
        p[6] = 0;
        if (DAT_8009c984 & 0x100) {
            if (a == 1)
                FUN_8004d620(0xf, 2);
            p[4] = 3;
            p[5] = 0;
            p[6] = 0;
        }
    }
}
