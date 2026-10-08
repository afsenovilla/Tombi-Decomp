// FUNC 80025d90 432 MAIN0
extern unsigned char DAT_8009d618;
extern unsigned char DAT_8009d619;
extern unsigned short DAT_8009d670;
extern unsigned short DAT_8009d674;
extern unsigned short DAT_800a6114;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8001fe;

void FUN_80025d90(void)
{
    unsigned a, b;
    unsigned short r, s, m;
    unsigned char c;
    a = DAT_8009d618;
    DAT_8009d618 = 0;
    DAT_8009d674 = DAT_800a6114;
    r = 0;
    m = DAT_8009d670 & 0xff0f;
    if (a - 0x50 >= 0x60) {
        b = (a - 0x30) & 0xffff;
        if (b < 0xa0) {
            DAT_8009d618 = 1;
            c = b < 0x50;
        } else {
            b = (a - 0x10) & 0xffff;
            if (b < 0xe0) {
                DAT_8009d618 = 2;
                c = b < 0x70;
            } else {
                DAT_8009d618 = 3;
                c = a < 0x80;
            }
        }
        r = 0x20;
        if (c) r = 0x80;
    }
    a = DAT_8009d619;
    DAT_8009d619 = 0;
    if (a - 0x50 < 0x60) {
        s = 0;
    } else {
        b = (a - 0x30) & 0xffff;
        if (b < 0xa0) {
            DAT_8009d619 = 1;
            c = b < 0x50;
        } else {
            b = (a - 0x10) & 0xffff;
            if (b < 0xe0) {
                DAT_8009d619 = 2;
                c = b < 0x70;
            } else {
                DAT_8009d619 = 3;
                c = a < 0x80;
            }
        }
        s = 0x40;
        if (c) s = 0x10;
    }
    m = m | r | s;
    DAT_8009d670 = m;
    DAT_1f8001fc = DAT_8009d670 & ~DAT_8009d674;
    DAT_1f8001fe = DAT_8009d674 & ~DAT_8009d670;
    DAT_800a6114 = DAT_8009d670;
}
