// FUNC 800fbbcc 104 I0NFO2
// MATCHING 800fbbcc 104
#include "TOBJ.H"
extern unsigned char D_8009CFA7, D_8009CFA8, D_8009CFAA;

void npcSet0EPickStep(TObj *o)
{
    if (D_8009CFA8 == 1) {
        o->step = 2;
        o->state = 0;
    } else if (D_8009CFA7 == 1) {
        D_8009CFA7++;
        o->step = 1;
        o->state = 0;
    } else if (D_8009CFAA == 1) {
        D_8009CFAA++;
        o->step = 3;
        o->state = 0;
    }
}
