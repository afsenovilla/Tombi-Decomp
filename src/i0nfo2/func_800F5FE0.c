// FUNC 800f5fe0 500 I0NFO2
// MATCHING 800f5fe0 500
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned char D_8009CDAB;
extern unsigned char D_8009CDF6;
extern unsigned char D_8009CE3B;
extern unsigned char D_8009CFCA;
extern unsigned char D_8009CFFF;

short func_800F5FE0(TObj *o)
{
    Pos6 p;
    short r;
    p = *(Pos6 *)&o->a;
    r = 0;
    switch (U16(o, 0xBA)) {
    case 0:
        S32(o, 0x90) = func_8002DCC8(3, 9, &p);
        r++;
        break;
    case 1:
        S32(o, 0x90) = func_8002DCC8(3, 0xA, &p);
        r++;
        break;
    case 2:
        S32(o, 0x90) = func_8002DCC8(3, 0xB, &p);
        r++;
        break;
    case 3:
        if (D_8009CFCA == 4) {
            if (D_8009CE3B != 0xFF) {
                if (D_8009CFFF == 0) {
                    S32(o, 0x90) = func_8002DCC8(3, 5, &p);
                    o->state = 7;
                    D_8009CFFF = 1;
                } else {
                    S32(o, 0x90) = func_8002DCC8(3, 6, &p);
                }
            } else {
                S32(o, 0x90) = func_8002DCC8(3, 6, &p);
            }
        } else if (D_8009CE3B == 0xFF) {
            S32(o, 0x90) = func_8002DCC8(3, 6, &p);
        } else if (D_8009CE3B != 0) {
            S32(o, 0x90) = func_8002DCC8(3, 4, &p);
        } else {
            S32(o, 0x90) = func_8002DCC8(3, 3, &p);
            o->state = 4;
        }
        r++;
        break;
    case 4:
        if (D_8009CDF6 == 0xFF) {
            S32(o, 0x90) = func_8002DCC8(3, 2, &p);
        } else {
            S32(o, 0x90) = func_8002DCC8(3, 0, &p);
            o->wac = 0x52;
            o->state = 2;
        }
        r++;
        break;
    case 5:
        if (D_8009CDAB != 0xFF) {
            S32(o, 0x90) = func_8002DCC8(3, 0xC, &p);
            o->state = 9;
        } else {
            S32(o, 0x90) = func_8002DCC8(3, 0xD, &p);
        }
        r++;
        break;
    }
    return r;
}
