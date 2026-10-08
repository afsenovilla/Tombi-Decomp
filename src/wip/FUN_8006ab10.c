// FUNC 8006ab10 104 MAIN0
extern int (*DAT_800981c8)(void);
extern char LAB_8006ab78[], LAB_8006ab94[];

int FUN_8006ab10(char *o, int a)
{
    int r = DAT_800981c8();
    if (r == 0) {
        o[0x46] = 1;
        *(char **)(o + 0x14) = LAB_8006ab78;
        *(int *)(o + 0x20) = a;
        *(char **)(o + 0x18) = LAB_8006ab94;
        return 1;
    }
    return 0;
}
