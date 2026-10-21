// FUNC 800fc050 368 I0NFO2
// MATCHING 800fc050 368
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void spawnNpcSet0E(TObj *o);
extern void func_800FB758(TObj *o);
extern void func_800FBC34(TObj *o);
extern void func_800FBD6C(TObj *o);
extern unsigned char D_8009CFA7, D_8009CFA8, D_8009CFAA;

void updateNpcSet0E(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet0E(o);
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
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
            break;
        case 1:
            func_800FB758(o);
            break;
        case 2:
            func_800FBC34(o);
            break;
        case 3:
            func_800FBD6C(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
