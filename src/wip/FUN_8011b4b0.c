// FUNC 8011b4b0 300 X000
extern unsigned char DAT_a, DAT_b, DAT_c, DAT_d, DAT_e, DAT_f;
extern int FUN_8002dc50(int, int, int, int);
extern void FUN_80018980(void *);

void FUN_8011b4b0(unsigned char *o)
{
    switch (o[5]) {
    case 0:
        if (o[1] == 0)
            o[5] = 2;
        else
            o[5] = o[5] + 1;
        break;
    case 1:
        if (DAT_a == 0) {
            DAT_b = 1;
            DAT_c = 1;
            DAT_d = 0;
            o[5] = o[5] + 1;
        }
        break;
    case 2:
        o[5] = o[5] + 1;
        *(int *)(o + 0x1c) = FUN_8002dc50(10, 0, 100, 236);
        break;
    case 3: {
        unsigned char *q = *(unsigned char **)(o + 0x1c);
        if (q[4] == 2) {
            q[4] = 3;
            DAT_b = 0;
            DAT_e = 0;
            DAT_c = 0;
            FUN_80018980(o);
        }
        break;
    }
    }
}
