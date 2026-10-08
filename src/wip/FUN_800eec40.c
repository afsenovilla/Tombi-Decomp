// FUNC 800eec40 512 X000
extern void FUN_8001fe6c(unsigned char *), FUN_8001fe94(unsigned char *, int);
extern int DAT_8009c960, DAT_8009c944;

void FUN_800eec40(unsigned char *o)
{
    int u;
    int i;
    unsigned x;
    u = **(unsigned short **)(o + 0x24);
    switch (u) {
    case 0x47:
        break;
    case 0xfa:
    case 0xfb:
    case 0xfc:
    {
            if ((*(unsigned short *)(o + 0x2e) & 1) == 0) {
                i = *(int *)(o + 0x8c);
                if (i < 0) {
                    *(int *)(o + 0x8c) = -i;
                    i = *(int *)(o + 0x8c);
                }
                *(int *)(o + 0x8c) = i + 4;
                if (i + 4 < 0x21)
                    return;
                *(int *)(o + 0x8c) = 0x20;
                return;
            }
            if (*(int *)(o + 0x8c) > 0)
                *(int *)(o + 0x8c) = -*(int *)(o + 0x8c);
            i = *(int *)(o + 0x8c) - 4;
            *(int *)(o + 0x8c) = i;
            if (i > -0x21)
                return;
            *(int *)(o + 0x8c) = -0x20;
            return;
            }
    default:
    {
        if ((*(unsigned short *)(o + 0x2e) & 1) == 0) {
            if (*(int *)(o + 0x8c) < 0x80)
                *(int *)(o + 0x8c) = (0x100 - *(int *)(o + 0x8c)) & 0xff;
            x = (*(int *)(o + 0x8c) - 4) & 0xff;
            *(int *)(o + 0x8c) = x;
            if (x > 0xbf)
                return;
        } else {
            if (*(int *)(o + 0x8c) > 0x80)
                *(int *)(o + 0x8c) = (0x100 - *(int *)(o + 0x8c)) & 0xff;
            x = (*(int *)(o + 0x8c) + 4) & 0xff;
            *(int *)(o + 0x8c) = x;
            if (x < 0x41)
                return;
        }
        if (DAT_8009c960 == 3 && DAT_8009c944 != 0) {
            *(int *)(o + 0x24) = 0x800112e0;
            FUN_8001fe6c(o);
            *(int *)(o + 0x8c) = 0;
            return;
        }
        *(int *)(o + 0x24) = 0x80010ae8;
        FUN_8001fe94(o, 3);
    }
    }
    *(int *)(o + 0x8c) = 0;
}
