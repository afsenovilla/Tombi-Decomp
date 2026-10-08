// FUNC 8012f094 120 X004
// MATCHING 8012f094 120
#include "TOBJ.H"
extern unsigned char D_8009CDDC;
extern unsigned char D_8009CF24[];

void func_8012F094(TObj *o)
{
    short i;

    if (o->state == 0) {
        o->step = 0;
        o->state = 0;
        if (D_8009CDDC != 0xff) {
            for (i = 0; i < 5; i++) {
                if (D_8009CF24[i] == 0) return;
            }
            o->step = 2;
        }
    }
}
