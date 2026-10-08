// FUNC 80057d04 248 MAIN0
extern char DAT_1f8000c0[];
extern int DAT_8009c960;
extern int DAT_1f8001e0;
extern unsigned char DAT_800b0d98[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void FUN_80120280(int, int);
extern void FUN_80024a70(int, int);

void FUN_80057d04(void)
{
    int i;
    int *p;
    unsigned char *base;
    SetRotMatrix(DAT_1f8000c0);
    SetTransMatrix(DAT_1f8000c0);
    base = DAT_800b0d98;
    p = (int *)base;
    if (DAT_8009c960 == 0x60009) {
        i = 0;
        if (DAT_800b0d98[3] != 0) {
            do {
                FUN_80120280(p[1], DAT_1f8001e0 + 0x10);
                p++;
                i++;
            } while (i < base[3]);
        }
    } else {
        i = 0;
        if (DAT_800b0d98[3] != 0) {
            do {
                FUN_80024a70(p[1], DAT_1f8001e0 + 0x10);
                p++;
                i++;
            } while (i < base[3]);
        }
    }
}
