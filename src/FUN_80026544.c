// FUNC 80026544 1228 MAIN0
// MATCHING 80026544 1228
#include "TOBJ.H"
#include "raw7.h"
extern Fix16 *DAT_8009c96c;
extern unsigned char DAT_8009c970, DAT_8009c971;
extern unsigned char DAT_8009c990[];
extern unsigned char DAT_800b1468[];
extern unsigned char DAT_8009d0a4[];
extern short DAT_8007977c[];
extern short DAT_80079784[];
extern unsigned char DAT_800a6104[];
extern unsigned char DAT_8009c970_[], DAT_8009c971_[];

void FUN_80026544(TObj *o)
{
    int i;
    unsigned char *p;

    switch (o->b04) {
    case 0:
        o->w08 = 0;
        o->b04++;
        break;
    case 1:
    if (o->w08 != 0)
        o->w08--;
    if (DAT_8009c96c != o->d)
        o->d = DAT_8009c96c;
    if (DAT_8009c970 != o->w50) {
        o->w50 = DAT_8009c970;
        o->w4e = 6;
        if (o->w4a == 0)
            o->w4a++;
    }
    o->w48 = 1;
    switch (o->w4a) {
    case 0:
        if (--o->w4c <= 0) {
            o->w4c = 0;
            if (DAT_8009c971_[0] - 1 < DAT_8009c970_[0])
                o->w48 = 0;
        }
        break;
    case 1:
        o->w4c = 6;
        o->w52 = S16(o, 0x54);
        o->w4a++;
        break;
    case 2:
        if (--o->w4c <= 0)
            o->w4a++;
        break;
    case 3:
        o->w4c = 6;
        o->w52 = o->w50;
        o->w4a++;
        break;
    case 4:
        if (--o->w4c <= 0) {
            if (--o->w4e <= 0) {
                o->w4c = 0xb4;
                o->w4a = 0;
                S16(o, 0x54) = o->w50;
            } else {
                o->w4a = 1;
            }
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        if (DAT_8009d0a4[DAT_8007977c[i]] != 0 && DAT_8009c990[0] == DAT_80079784[i]) {
            switch (DAT_800a6104[0]) {
            case 0:
                if (--DAT_800b1468[4 + i] == 0) {
                    DAT_800b1468[4 + i] = 3;
                    if (++DAT_8009c990[0x40f] >= 0x3c) {
                        DAT_800a6104[0] = 1;
                        DAT_8009c990[0x40f] = 0x3c;
                        DAT_800b1468[4 + i] = 0;
                        DAT_800b1468[12 + i] = 1;
                        DAT_800b1468[8 + i] = 6;
                    }
                }
                break;
            case 1:
                break;
            case 2:
                DAT_800b1468[4 + i] = 0x78;
                DAT_800b1468[8 + i] = 0;
                DAT_800b1468[12 + i] = 0;
                DAT_8009c990[0x40f] = 0;
                break;
            case 3:
                DAT_800b1468[8 + i] = 0;
                if (--DAT_800b1468[4 + i] == 0) {
                    DAT_800a6104[0] = 0;
                    DAT_800b1468[4 + i] = 1;
                }
                break;
            }
            if (DAT_800b1468[8 + i] != 0 && --DAT_800b1468[8 + i] == 0) {
                if (++DAT_800b1468[12 + i] >= 4)
                    DAT_800b1468[12 + i] = 0;
                DAT_800b1468[8 + i] = 6;
            }
        } else {
            if (DAT_800b1468[4 + i] != 0) {
                DAT_800b1468[i] = 0x78;
                DAT_800b1468[4 + i]--;
                if (++DAT_8009c990[0x40c + i] >= 0x3c) {
                    if (DAT_8009c990[0x408 + i] == 9) {
                        DAT_8009c990[0x40c + i] = 0x3c;
                        DAT_800b1468[4 + i] = 0;
                    } else {
                        DAT_8009c990[0x408 + i]++;
                        DAT_8009c990[0x40c + i] -= 0x3c;
                        DAT_800b1468[12 + i] = 1;
                        DAT_800b1468[8 + i] = 6;
                    }
                }
            }
            if (DAT_800b1468[i] != 0)
                DAT_800b1468[i]--;
            p = (unsigned char *)o + i;
            if (p[0x64] != 0 && --p[0x60] == 0) {
                if (++p[0x64] >= 4)
                    p[0x64] = 0;
                p[0x60] = 6;
            }
        }
    }
        break;
    case 2:
        o->b04++;
        break;
    }
}
