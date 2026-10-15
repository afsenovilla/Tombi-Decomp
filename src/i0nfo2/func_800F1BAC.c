// FUNC 800f1bac 220 I0NFO2
// MATCHING 800f1bac 220
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned char D_8009CDF3[];
extern unsigned char D_8009CFA9;

int func_800F1BAC(TObj *o)
{
    Pos6 p;
    p = *(Pos6 *)&o->a;
    if (D_8009CDF3[0] == 0) {
        if (D_8009CFA9 == 0) {
            S32(o, 0x90) = func_8002DCC8(2, 4, &p);
            D_8009CFA9 = D_8009CFA9 + 1;
        } else {
        S32(o, 0x90) = func_8002DCC8(2, 5, &p);
        o->state = 2;
        o->wac = 0x4F;
 }
    } else {
        S32(o, 0x90) = func_8002DCC8(2, 5, &p);
    }
    return 1;
}
