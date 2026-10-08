// FUNC 80125f30 224 X000
extern short FUN_80042fbc(void);
extern int *DAT_8009c330;

void FUN_80125f30(unsigned char *o, unsigned char *p)
{
    unsigned short u;

    if (FUN_80042fbc() != 0) {
        p[0x69] = 1;
        if (o[0x9c] == 1 && *(unsigned short *)((char *)DAT_8009c330 + 0x20) == 1) {
            *(short *)((char *)DAT_8009c330 + 0x20) = 0xe;
            *(short *)(o + 0x7e) = -1000;
        }
        u = *(unsigned short *)(o + 0xb2);
        if ((unsigned short)(u + 0x100) > 0x200) {
            if ((short)u < 0) {
                *(unsigned short *)(o + 0xb2) = u + 0x30;
                if ((short)(u + 0x30) > -0x100)
                    *(short *)(o + 0xb2) = -0x100;
            } else {
                *(unsigned short *)(o + 0xb2) = u - 0x30;
                if ((short)(u - 0x30) < 0x100)
                    *(short *)(o + 0xb2) = 0x100;
            }
        }
    }
}
