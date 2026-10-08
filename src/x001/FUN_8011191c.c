// FUNC 8011191c 308 X001
// MATCHING 8011191c 308
extern unsigned char DAT_a;
extern void FUN_8011145c(void *);
extern void FUN_800202b4(void *);
extern void FUN_80111590(void *);
extern void FUN_80111714(void *);
extern void FUN_80018744(void *);

void FUN_8011191c(unsigned char *o)
{
    switch (o[4]) {
    case 0:
        FUN_8011145c(o);
        break;
    case 1:
        if ((unsigned)(DAT_a - 4) >= 2) {
            FUN_800202b4(o);
            switch (o[3]) {
            case 0:
                FUN_80111590(o);
                break;
            case 1:
                FUN_80111714(o);
                break;
            }
            if (*(short *)(o + 0x22) != 0) {
                *(short *)(o + 0x22) = *(short *)(o + 0x22) - 1;
                if (*(short *)(o + 0x22) < 1)
                    o[0] = 1;
            }
            if (o[0x6a] != 0) {
                o[0x6a] = 0;
                *(short *)(o + 0x22) = 3;
            }
        }
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        FUN_80018744(o);
        break;
    }
}
