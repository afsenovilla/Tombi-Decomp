// FUNC 8001801c 220 MAIN0
// MATCHING 8001801c 220
extern char DAT_800b1828[];
extern char DAT_800b3c98[];
extern char DAT_800a6264[];
extern char DAT_800b3e24[];
extern char *DAT_1f80020c;
extern char *DAT_1f800230;
extern char *DAT_1f800274;
extern short DAT_1f80023a;
extern short DAT_1f800258;
extern short DAT_1f800242;
extern void memset(char *, int, int);
extern void FUN_80018ebc(void);

void FUN_8001801c(void)
{
    char *p;
    char *q;
    int i;
    char **sp;

    i = 0;
    p = DAT_800b1828;
    do {
        memset(p, 0, 0xd4);
        i++;
        p += 0xd4;
    } while (i < 0x2d);
    q = DAT_800b3c98;
    DAT_1f80020c = DAT_800a6264;
    i = 0;
    do {
        q[0x1c] = 8;
        i++;
        sp = (char **)DAT_1f80020c - 1;
        DAT_1f80020c = (char *)sp;
        *sp = q;
        q -= 0xd4;
    } while (i < 0x2d);
    DAT_1f80023a = 0x2d;
    DAT_1f800230 = DAT_800b3e24;
    DAT_1f800274 = DAT_800b3e24;
    DAT_1f800258 = 0;
    DAT_1f800242 = 0;
    FUN_80018ebc();
}
