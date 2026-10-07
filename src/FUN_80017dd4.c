// FUNC 80017dd4 212 MAIN0
// MATCHING 80017dd4 212
extern unsigned char DAT_800b1478[];
extern unsigned char DAT_800b173c[];
extern unsigned char DAT_800a4550[];
extern unsigned char DAT_800b11c8[];
extern unsigned char **DAT_1f800204;
extern short DAT_1f800236;
extern unsigned char *DAT_1f800218;
extern unsigned char *DAT_1f80025c;
extern short DAT_1f80024e;
extern short DAT_1f800244;
extern void *memset(void *, int, int);

void FUN_80017dd4(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = DAT_800b1478;
    do {
        memset(p, 0, 0xec);
        i++;
        p += 0xec;
    } while (i < 4);
    q = DAT_800b173c;
    DAT_1f800204 = (unsigned char **)DAT_800a4550;
    i = 0;
    do {
        q[0x1c] = 1;
        i++;
        *--DAT_1f800204 = q;
        q -= 0xec;
    } while (i < 4);
    DAT_1f800236 = 4;
    DAT_1f800218 = DAT_800b11c8;
    DAT_1f80025c = DAT_800b11c8;
    DAT_1f80024e = 0;
    DAT_1f800244 = 0;
}
