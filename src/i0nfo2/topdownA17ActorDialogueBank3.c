// FUNC 800f3f80 400 I0NFO2
// MATCHING 800f3f80 400
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned char D_8009CDF0;
extern unsigned char D_8009CE05;
extern unsigned char D_8009CE0A;
extern unsigned char D_8009CE0D;
extern unsigned char D_8009CFA6;
extern unsigned char D_8009CFAB;
extern unsigned char D_8009D11A;

int topdownA17ActorDialogueBank3(TObj *o)
{
    Pos6 p;
    p = *(Pos6 *)&o->a;
    if (D_8009CE05 != 0xFF) {
        S32(o, 0x90) = func_8002DCC8(3, 0, &p);
        return 1;
    }
    if (D_8009CE0D == D_8009CE05) {
        S32(o, 0x90) = func_8002DCC8(3, 0xB, &p);
        return 1;
    }
    if (D_8009CE0D == 0) {
        if (D_8009CE0A != 0) {
            S32(o, 0x90) = func_8002DCC8(3, 3, &p);
            if (D_8009CE0D == 0) {
                o->state = 2;
                o->wac = 0x69;
                return 1;
            }
        } else {
            if (D_8009CFA6 == 0) {
                D_8009CFA6++;
                return 0;
            }
            S32(o, 0x90) = func_8002DCC8(3, 2, &p);
            return 1;
        }
    } else if (D_8009CE0D == 1) {
        D_8009CE0D++;
        return 0;
    } else if (D_8009CDF0 == D_8009CE05) {
        if (D_8009D11A != 0) {
            D_8009CFAB = 1;
            return 0;
        }
        S32(o, 0x90) = func_8002DCC8(3, 8, &p);
        return 1;
    } else {
        S32(o, 0x90) = func_8002DCC8(3, 5, &p);
        return 1;
    }
}
