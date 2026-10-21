// FUNC 800f1c88 448 I0NFO2
// MATCHING 800f1c88 448
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned char D_8009CDF3;
extern unsigned char D_8009CFA5;
extern unsigned char D_8009CFA9[];

short topdownA11ActorOpenDialogue(TObj *o)
{
    Pos6 p;
    short r;
    p = *(Pos6 *)&o->a;
    r = 0;
    o->wac = 0;
    switch (U16(o, 0xBA)) {
    case 0:
        S32(o, 0x90) = func_8002DCC8(3, 0, &p);
        r++;
        break;
    case 1:
        if (D_8009CFA9[0] == 0) {
            if (D_8009CDF3 == 0) {
                D_8009CFA9[0] = 1;
            } else {
                D_8009CFA9[0] = 2;
            }
        }
        break;
    case 2:
        S32(o, 0x90) = func_8002DCC8(4, 2, &p);
        r++;
        break;
    case 3:
        S32(o, 0x90) = func_8002DCC8(4, 0, &p);
        r++;
        break;
    case 4:
        S32(o, 0x90) = func_8002DCC8(4, 1, &p);
        r++;
        break;
    case 5:
        S32(o, 0x90) = func_8002DCC8(4, 2, &p);
        r++;
        break;
    case 6:
        S32(o, 0x90) = func_8002DCC8(4, 3, &p);
        r++;
        break;
    case 7:
        S32(o, 0x90) = func_8002DCC8(4, 4, &p);
        r++;
        break;
    case 8:
        switch (D_8009CFA5) {
        case 0:
            S32(o, 0x90) = func_8002DCC8(4, 5, &p);
            break;
        case 1:
            S32(o, 0x90) = func_8002DCC8(4, 0xA, &p);
            break;
        case 2:
            S32(o, 0x90) = func_8002DCC8(4, 0x10, &p);
            break;
        }
        r++;
        break;
    case 9:
        S32(o, 0x90) = func_8002DCC8(4, 6, &p);
        r++;
        break;
    case 10:
        if (D_8009CFA5 == 0) {
            S32(o, 0x90) = func_8002DCC8(4, 7, &p);
        } else {
            S32(o, 0x90) = func_8002DCC8(4, 0xB, &p);
        }
        r++;
        break;
    }
    return r;
}
