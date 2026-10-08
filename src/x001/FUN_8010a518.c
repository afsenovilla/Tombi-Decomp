// FUNC 8010a518 1244 X001
// MATCHING 8010a518 1244
#include "TOBJ.H"
typedef struct { short v[9]; } T9;
extern T9 D_800E98B0, D_800E98C4, D_800E98D8;
extern unsigned char *D_8009C330;
extern unsigned short D_8009D670;

void FUN_8010a518(TObj *o)
{
    T9 t1 = D_800E98B0;
    T9 t2 = D_800E98C4;
    T9 t3 = D_800E98D8;
    volatile unsigned short *k;
    D_8009C330[0xb] = 8;
    if (o->animFrame & 1) {
        k = &D_8009D670;
        if (*k & 0x10) {
            D_8009C330[0xb] = 0;
            if (*k & 0x80) D_8009C330[0xb] = 7;
            if (*k & 0x20) D_8009C330[0xb] = 1;
        } else if (*k & 0x80) {
            D_8009C330[0xb] = 6;
            if (*k & 0x10) D_8009C330[0xb] = 7;
            if (*k & 0x40) D_8009C330[0xb] = 5;
        } else if (*k & 0x40) {
            D_8009C330[0xb] = 4;
            if (*k & 0x80) D_8009C330[0xb] = 5;
            if (*k & 0x20) D_8009C330[0xb] = 3;
        } else if (*k & 0x20) {
            D_8009C330[0xb] = 2;
            if (*k & 0x10) D_8009C330[0xb] = 1;
            if (*k & 0x40) D_8009C330[0xb] = 3;
        }
    } else {
        k = &D_8009D670;
        if (*k & 0x10) {
            D_8009C330[0xb] = 0;
            if (*k & 0x80) D_8009C330[0xb] = 1;
            if (*k & 0x20) D_8009C330[0xb] = 7;
        } else if (*k & 0x80) {
            D_8009C330[0xb] = 2;
            if (*k & 0x10) D_8009C330[0xb] = 1;
            if (*k & 0x40) D_8009C330[0xb] = 3;
        } else if (*k & 0x40) {
            D_8009C330[0xb] = 4;
            if (*k & 0x80) D_8009C330[0xb] = 3;
            if (*k & 0x20) D_8009C330[0xb] = 5;
        } else if (*k & 0x20) {
            D_8009C330[0xb] = 6;
            if (*k & 0x10) D_8009C330[0xb] = 7;
            if (*k & 0x40) D_8009C330[0xb] = 5;
        }
    }
    switch (o->wb2) {
    case 1:
        o->d88 = t1.v[D_8009C330[0xb]];
        break;
    case 2:
        o->d88 = t2.v[D_8009C330[0xb]];
        break;
    case 3:
        o->d88 = t3.v[D_8009C330[0xb]];
        break;
    }
}
