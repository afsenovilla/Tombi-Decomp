// FUNC 80018ebc 172 MAIN0
extern char DAT_800b0bb8[];
extern char DAT_800b0bba[];
extern char DAT_800b0bbc[];
extern char DAT_800b0bbd[];
extern char DAT_800b0bbe[];
extern char DAT_800b0bbf[];
extern char DAT_800b0bc0[];
extern char DAT_800b0bc1[];

void FUN_80018ebc(void)
{
    int a = 0;
    char c = -0x62;
    int i = 0;
    int k;
    for (k = 0;; k += 10) {
        DAT_800b0bbc[k] = a;
        a += 4;
        *(short *)(DAT_800b0bb8 + k) = -1;
        *(short *)(DAT_800b0bba + k) = 0;
        DAT_800b0bbd[k] = c;
        DAT_800b0bbe[k] = 4;
        DAT_800b0bbf[k] = 0x18;
        DAT_800b0bc0[k] = 0;
        DAT_800b0bc1[k] = 0;
        if (a > 0x3b) {
            a = 0;
            c += 0x18;
        }
        i++;
        if (i >= 0x30) break;
    }
}
