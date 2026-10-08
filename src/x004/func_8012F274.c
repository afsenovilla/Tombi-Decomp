// FUNC 8012f274 220 X004
// MATCHING 8012f274 220
#include "TOBJ.H"
extern unsigned char D_8009CDDC;
extern unsigned char D_8009CF24[];
extern void func_8012EE14(TObj *);
extern void func_8012F10C(TObj *);

void func_8012F274(TObj *o)
{
    short i;
    switch (o->step) {
    case 0:
        func_8012EE14(o);
        break;
    case 1:
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
        break;
    case 2:
        func_8012F10C(o);
        break;
    }
}
