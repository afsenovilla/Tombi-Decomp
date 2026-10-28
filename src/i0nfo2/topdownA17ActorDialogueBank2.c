// FUNC 800f3ea0 224 I0NFO2
// MATCHING 800f3ea0 224
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned char D_8009CE05[], D_8009CE06, D_8009CE07;

int topdownA17ActorDialogueBank2(TObj *o)
{
    Pos6 p;
    p = *(Pos6 *)&o->a;
    if (D_8009CE05[0] != 0) {
        if (D_8009CE05[0] == 0xFF) {
            o->d90 = func_8002DCC8(2, 10, &p);
        } else if (D_8009CE06 == 0) {
            o->d90 = func_8002DCC8(2, 5, &p);
        } else if (D_8009CE07 != 0) {
            o->d90 = func_8002DCC8(2, 9, &p);
        } else {
            o->d90 = func_8002DCC8(2, 6, &p);
            o->state = 2;
            o->wac = 0x63;
        }
    } else {
        o->d90 = func_8002DCC8(2, 5, &p);
    }
    return 1;
}
