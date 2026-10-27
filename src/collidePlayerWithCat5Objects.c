// FUNC 80047a3c 180 MAIN0
// MATCHING 80047a3c 180
#include "gvars.h"
#include "TOBJ.H"
extern signed char GAME_PLAYER_MODE;
extern short D_1f80019e;
extern unsigned short CAT5_OBJECT_COUNT;
extern unsigned char **CAT5_OBJECT_LIST;
extern void collidePlayerWithCat5Object(TObj *o, unsigned char *p);
void collidePlayerWithCat5Objects(TObj *o)
{
    unsigned char **pp;
    unsigned char *p;
    if (GVARS.b38 != 0) return;
    if (GAME_PLAYER_MODE == 3) return;
    pp = CAT5_OBJECT_LIST;
    D_1f80019e = CAT5_OBJECT_COUNT;
    while (D_1f80019e != 0) {
        p = *pp++;
        D_1f80019e -= 1;
        if (*p & 3) {
            collidePlayerWithCat5Object(o, p);
        }
    }
}
