// FUNC 8004bfe4 248 MAIN0
// MATCHING 8004bfe4 248
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern unsigned char **PTR_8007bee4[];
extern void (*PTR_8007bf34[])(unsigned char *);
extern unsigned char *FUN_800186f8(void);

void FUN_8004bfe4(void)
{
    unsigned char *e;
    unsigned char *p;

    p = PTR_8007bee4[DAT_8009c960][DAT_8009c962];
    while (*p != 0xff) {
        e = FUN_800186f8();
        if (e) {
            *e = 1;
            e[2] = p[0];
            e[3] = p[1];
            if (p[3]) PTR_8007bf34[e[2]](e);
        }
        p += 4;
    }
}
