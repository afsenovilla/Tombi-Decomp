// FUNC 8011de60 408 X004
// MATCHING 8011de60 408
extern void FUN_8001fe6c(unsigned char *);
extern short FUN_800411cc(unsigned char *, int, int);
extern int DAT_8013b104;
extern char DAT_80077d54[];

void func_8011DE60(unsigned char *o)
{
    short v;
    switch (o[5]) {
    case 0:
        if ((*(unsigned short *)(o + 0x2e) & 2) == 0)
            *(short *)(o + 0x7e) = -0x200;
        else
            *(short *)(o + 0x7e) = 0x200;
        *(char **)(o + 0x28) = DAT_80077d54;
        *(int *)(o + 0x24) = DAT_8013b104;
        FUN_8001fe6c(o);
        o[5] = o[5] + 1;
    case 1:
        if (*(unsigned short *)(o + 0x2e) & 1)
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) - 0x14) & 0xff;
        else
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) + 0x14) & 0xff;
        *(int *)(o + 0x14) = *(int *)(o + 0x14) + (*(short *)(o + 0x7e) << 8);
        v = *(unsigned short *)(o + 0x7e) + 0x50;
        *(short *)(o + 0x7e) = v;
        if (v > 0)
            o[5] = o[5] + 1;
        break;
    case 2:
        if (*(unsigned short *)(o + 0x2e) & 1)
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) - 0x14) & 0xff;
        else
            *(int *)(o + 0x8c) = (*(int *)(o + 0x8c) + 0x14) & 0xff;
        *(int *)(o + 0x14) = *(int *)(o + 0x14) + (*(short *)(o + 0x7e) << 8);
        *(short *)(o + 0x7e) += 0x20;
        if (FUN_800411cc(o, (*(short **)(o + 0x40))[1], *(short *)(o + 0x16)) != 0) {
            o[4] = 2;
            o[5] = 0;
        }
        break;
    }
}
