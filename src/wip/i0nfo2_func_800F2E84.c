// FUNC 800f2e84 468 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned char D_8009CDE1;
extern unsigned char D_8009CDE2;
extern unsigned char D_8009CE0B;
extern unsigned char D_8009CF60;
extern unsigned char D_8009CFA7[];
extern unsigned char D_8009CFAA;
extern unsigned char D_800FEF24[][4];

short func_800F2E84(TObj *o)
{
    Pos6 p;
    short r;
    p = *(Pos6 *)&o->a;
    r = 0;
    o->wac = 0;
    switch (U16(o, 0xBA)) {
    case 8: {
        Pos6 q;
        int x;
        q = *(Pos6 *)&o->a;
        x = 2;
        if (D_8009CDE2 == 0) {
            x = 0;
            if (D_8009CDE1 != 0) goto inc;
        }
        S32(o, 0x90) = func_8002DCC8(2, x, &q);
        r = 1;
        break;
    inc:
        D_8009CFAA++;
        r = 0;
        break;
    }
    case 9:
    case 10:
        if (D_8009CFA7[0] == 0) {
            D_8009CFA7[0] = 1;
        }
        break;
    case 11:
        break;
    case 12:
        S32(o, 0x90) = func_8002DCC8(2, 0x15, &p);
        if (D_8009CE0B == 0) {
            o->state = 2;
            o->wac = 0x67;
        }
        r++;
        break;
    case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
    case 13: case 14:
        S32(o, 0x90) = func_8002DCC8(3, D_800FEF24[o->b0c][D_8009CF60], &p);
        r++;
        break;
    case 15:
        S32(o, 0x90) = func_8002DCC8(2, 0x15, &p);
        r++;
        break;
    }
    return r;
}
