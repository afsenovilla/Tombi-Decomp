// FUNC 8004bfe4 248 MAIN0
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern unsigned char **PTR_8007bee4[];
extern void (*PTR_8007bf34[])(void);
extern unsigned char *FUN_800186f8(void);

void FUN_8004bfe4(void)
{
    unsigned char *q;
    unsigned char *e;
    unsigned char *p;

    p = PTR_8007bee4[DAT_8009c960][DAT_8009c962];
    if (*p != 0xff) {
        q = p + 3;
        do {
            e = FUN_800186f8();
            if (e) {
                *e = 1;
                e[2] = *p;
                e[3] = q[-2];
                if (*q) PTR_8007bf34[e[2]]();
            }
            q += 4;
            p += 4;
        } while (*p != 0xff);
    }
}
