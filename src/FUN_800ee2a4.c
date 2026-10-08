// FUNC 800ee2a4 304 X000
extern void FUN_800201ac(unsigned char *, int);
extern void FUN_80018838(void);

void FUN_800ee2a4(unsigned char *o)
{
    unsigned char t, s;
    short v;
    s = o[4];
    switch (s) {
    case 0:
        t = o[0xc];
        o[4] = s + 1;
        if (t == 0) {
            *(short *)(o + 0x6c) = 4;
            *(short *)(o + 0x6e) = 8;
            *(short *)(o + 0x70) = 0xaa;
            *(short *)(o + 0x72) = 0x154;
            o[3] = 1;
        } else {
            if (t == 1) {
                *(short *)(o + 0x6c) = 6;
                *(short *)(o + 0x6e) = 8;
                *(short *)(o + 0x70) = 0xaa;
                v = 0x154;
            } else if (t == 2) {
                *(short *)(o + 0x6c) = 6;
                *(short *)(o + 0x6e) = 8;
                *(short *)(o + 0x70) = 0x28;
                v = 0xb4;
            } else {
                if (t != 3)
                    goto end;
                *(short *)(o + 0x6c) = 6;
                *(short *)(o + 0x6e) = 8;
                *(short *)(o + 0x70) = 0x64;
                v = 0xc8;
            }
            *(short *)(o + 0x72) = v;
        }
    end:
        *(int *)(o + 0xa0) = 0;
        return;
    case 1:
        FUN_800201ac(o, 0x40);
        return;
    case 2:
        return;
    case 3:
        FUN_80018838();
        return;
    default:
        return;
    }
}
