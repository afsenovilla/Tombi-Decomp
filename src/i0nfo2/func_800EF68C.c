// FUNC 800ef68c 1756 I0NFO2
// MATCHING 800ef68c 1756
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
extern unsigned char D_8009CDC9;
extern unsigned char D_8009CDCA;
extern unsigned char D_8009CDCF;
extern unsigned char D_8009CDD2;
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CDDD;
extern unsigned char D_8009CEE5;
extern unsigned char D_8009CEE6;
extern unsigned char D_8009CEE7;
extern unsigned char D_8009CEEB;
extern unsigned char D_8009CEEC;
extern unsigned char D_8009CEED;
extern unsigned char D_8009CEEE;
extern unsigned char D_8009CEF2;
extern unsigned char D_8009CEF3;
extern unsigned char D_8009CF2A;
extern unsigned char D_8009CF2B;
extern unsigned char D_8009CF9D;
extern unsigned char D_8009CFA4;
extern unsigned char D_8009CFC5;
extern unsigned char D_8009D2C3;
extern TObj *showMessageBoxDirect(int, int, B12 *);

short func_800EF68C(TObj *o)
{
    B12 v;
    short r;

    v = *(B12 *)&o->a;
    r = 0;
    S16(o, 0xAC) = 0;
    switch ((unsigned short)o->wba) {
    case 0:
        S16(o, 0x2E) = o->b69 - 1;
        if (D_8009CDC9 != 0xFF) {
            if (D_8009CEE5 == 0) {
                S32(o, 0x90) = (int)showMessageBoxDirect(2, 1, &v);
                D_8009CEE5 = 1;
                r++;
            } else {
                S32(o, 0x90) = (int)showMessageBoxDirect(2, 3, &v);
                r++;
            }
        } else if (D_8009CDCA != D_8009CDC9) {
            D_8009CEE7 = 1;
            r++;
        } else if (D_8009CF2A != 2) {
            goto inc;
        } else {
            goto b;
        }
        break;
    case 1:
        if (D_8009CDC9 != 0xFF) {
            if (D_8009CEE6 == 0) {
                S32(o, 0x90) = (int)showMessageBoxDirect(2, 2, &v);
                D_8009CEE6 = 1;
                r++;
            } else {
                S32(o, 0x90) = (int)showMessageBoxDirect(2, 3, &v);
                r++;
            }
        } else if (D_8009CDCA != D_8009CDC9) {
            D_8009CEE7 = 1;
            r++;
        } else {
        b:
            D_8009CF2B = 1;
            r++;
        }
        break;
    case 2:
        D_8009CEEB = 1;
        r++;
        break;
    case 3:
        D_8009CEF3 = 1;
        break;
    case 4:
        D_8009CEF2 = 1;
        break;
    case 5:
        if (D_8009D2C3 & 0x10) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x54, &v);
        } else if (D_8009CDD5 == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x4A, &v);
        } else if (D_8009CEED >= 2) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x3F, &v);
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x38, &v);
        }
        r++;
        break;
    case 6:
        if (D_8009D2C3 & 0x10) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x55, &v);
        } else if (D_8009CDDD == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x50, &v);
        } else if (D_8009CDD5 == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x4B, &v);
        } else if (D_8009CEED < 2) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x39, &v);
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x40, &v);
        }
        r++;
        break;
    case 7:
        if (D_8009D2C3 & 0x10) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x56, &v);
        } else if (D_8009CDD5 == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x4C, &v);
        } else if (D_8009CEED >= 2) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x41, &v);
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x3A, &v);
        }
        r++;
        break;
    case 8:
        if (D_8009D2C3 & 0x10) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x57, &v);
        } else if (D_8009CDDD == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x51, &v);
        } else if (D_8009CDD5 == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x4D, &v);
        } else if (D_8009CEED < 2) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x3B, &v);
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x42, &v);
        }
        r++;
        break;
    case 9:
        if (D_8009CDD5 == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(1, 4, &v);
        } else {
            switch (D_8009CFC5) {
            case 0:
                S32(o, 0x90) = (int)showMessageBoxDirect(1, 1, &v);
                D_8009CFC5 = 1;
                break;
            case 1:
                S32(o, 0x90) = (int)showMessageBoxDirect(1, 2, &v);
                D_8009CFC5 = 2;
                break;
            case 2:
                S32(o, 0x90) = (int)showMessageBoxDirect(1, 3, &v);
                D_8009CFC5 = 0;
                S16(o, 0xAC) = 0x2C;
                U8(o, 6) = 2;
                break;
            }
        }
        r++;
        break;
    case 10:
        if (D_8009CDD5 == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(1, 4, &v);
        } else {
            switch (D_8009CFC5) {
            case 0:
                S32(o, 0x90) = (int)showMessageBoxDirect(1, 1, &v);
                D_8009CFC5 = 1;
                break;
            case 1:
                S32(o, 0x90) = (int)showMessageBoxDirect(1, 2, &v);
                D_8009CFC5 = 2;
                break;
            case 2:
                S32(o, 0x90) = (int)showMessageBoxDirect(1, 3, &v);
                D_8009CFC5 = 0;
                S16(o, 0xAC) = 0x2C;
                U8(o, 6) = 2;
                break;
            }
        }
        r++;
        break;
    case 11:
        if (D_8009D2C3 & 0x10) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x5C, &v);
            r++;
        } else if (D_8009CDD2 == 0xFF && D_8009CDD5 == D_8009CDD2) {
            D_8009CFA4 = 1;
            if (D_8009CF9D == 0) {
                D_8009CF9D++;
            }
        } else {
            D_8009CEEC = 1;
        }
        break;
    case 12:
        S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x3E, &v);
        r++;
        break;
    case 13:
        S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x24, &v);
        r++;
        break;
    case 14:
        S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x25, &v);
        r++;
        D_8009CDD2 = 2;
        U8(o, 6) = 2;
        break;
    case 15:
        S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x26, &v);
        r++;
        break;
    case 16:
        if (D_8009CDCF == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x20, &v);
            r++;
        } else {
            switch (D_8009CEEE) {
            case 0:
                S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x1C, &v);
                D_8009CEEE++;
                r++;
                break;
            case 1:
                S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x1D, &v);
                r++;
                D_8009CEEE++;
                S16(o, 0xAC) = 0x2B;
                U8(o, 6) = 2;
                break;
            case 2:
                S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x1E, &v);
                r++;
                break;
            }
        }
        break;
    case 17:
        if (D_8009CDDD == 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x53, &v);
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x44, &v);
        }
        r++;
        break;
    case 18:
        if (D_8009D2C3 & 0x10) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x5A, &v);
            r++;
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x45, &v);
            r++;
            S16(o, 0xAC) = 0xB;
            U8(o, 6) = 2;
        }
        break;
    case 20:
        U8(o, 6) = 6;
        r++;
        break;
    case 21:
        U8(o, 6) = 8;
        r++;
        break;
    case 22:
        if (D_8009D2C3 & 0x10) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x5B, &v);
        } else if (D_8009CDD5 != 0xFF) {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x46, &v);
        } else {
            S32(o, 0x90) = (int)showMessageBoxDirect(2, 0x4F, &v);
        }
    inc:
        r++;
        break;
    }
    return r;
}
