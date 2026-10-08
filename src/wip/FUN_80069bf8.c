// FUNC 80069bf8 240 MAIN0
/* score 51: library code from a newer compiler (jr ra; addiu sp in delay slot with s0 saved): not reproducible with CC1PSX 4.3. */
extern int DAT_800981f0;
extern int (*PTR_LAB_80098230[])(int *);
extern void (*DAT_800981ac)(int);
extern void FUN_8006bf84(int);
extern int FUN_8006a170(void);

void FUN_80069bf8(int *param_1)
{
    int r;
    r = PTR_LAB_80098230[DAT_800981f0++](param_1);
    if (r >= 0) {
        if (DAT_800981f0 != 0 && (DAT_800981f0 != 3 || **(unsigned char **)(param_1 + 0xf) != 0x80)) {
            FUN_8006bf84(0x3c);
            r = FUN_8006a170();
            if (r == 0)
                (*DAT_800981ac)(-3);
        }
        if (4 < DAT_800981f0)
            DAT_800981f0 = DAT_800981f0 - 1;
    } else {
        (*DAT_800981ac)(r);
    }
}
