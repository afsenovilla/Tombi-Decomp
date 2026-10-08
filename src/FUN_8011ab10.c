// FUNC 8011ab10 328 X000
// MATCHING 8011ab10 328
extern void FUN_800202b4(unsigned char *), FUN_8001fe6c(unsigned char *), FUN_800187e4(unsigned char *);
extern int FUN_8001fec0(unsigned char *);
extern unsigned char *PTR_8013b118;
extern int DAT_1f8002d4;
extern unsigned char *DAT_8009c330;

void FUN_8011ab10(unsigned char *o)
{
    unsigned char s;
    int t;
    unsigned char *u;
    s = o[4];
    switch (s) {
    case 0:
        o[4] = s + 1;
        *(short *)(o + 0x2e) = 0;
        *(short *)(o + 0x1e) = 0xc;
        t = DAT_1f8002d4;
        o[0xd] = 0;
        o[0xa] = 0;
        *(signed char *)(o + 0xf) = -7;
        u = PTR_8013b118;
        *(int *)(o + 0x3c) = t;
        *(unsigned char **)(o + 0x24) = u;
        FUN_8001fe6c(o);
        break;
    case 1:
        FUN_800202b4(o);
        switch (o[5]) {
        case 0:
            if (DAT_8009c330[10] == o[0xc]) {
                *(unsigned char **)(o + 0x24) = PTR_8013b118;
                FUN_8001fe6c(o);
                o[5]++;
            }
            break;
        case 1:
            if (FUN_8001fec0(o) != 0)
                o[5] = 0;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
