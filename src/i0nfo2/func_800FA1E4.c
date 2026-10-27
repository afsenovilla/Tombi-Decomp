// FUNC 800fa1e4 148 I0NFO2
// MATCHING 800fa1e4 148
#include "TOBJ.H"
extern void func_8005A8A8(int a, int b, int c);
extern unsigned char GVARS_PLAYER_LANDED, D_8009CDD9;

void func_800FA1E4(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_8009CDD9 == 0 && GVARS_PLAYER_LANDED != 0) {
            func_8005A8A8(0x35, 0, 0);
            o->state++;
        }
        break;
    case 1:
        o->step = 0;
        o->state = 0;
        break;
    }
}
