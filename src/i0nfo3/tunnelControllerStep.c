// FUNC 800eea70 424 I0NFO3
// MATCHING 800eea70 424
#include "TOBJ.H"
extern short tunnelCheckSlideZone(void);
extern void tunnelUseItem95Step(TObj *o);
extern void tunnelSlidePlayerStep(TObj *o);
extern void func_8005A9A4(int e, int a);
extern unsigned char GVARS_PLAYER_LANDED;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDF3;
extern unsigned char D_8009CE21;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned char D_800A60E4;

void tunnelControllerStep(TObj *o)
{
    switch (o->step) {
    case 0:
        if (tunnelCheckSlideZone() != 0) {
            o->step = 3;
            o->state = 0;
        } else if (D_8009CDF3 != 0xFF) {
            o->step = 2;
            o->state = 0;
        } else if (D_8009CE21 == 1) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        tunnelUseItem95Step(o);
        break;
    case 2:
        switch (o->state) {
        case 0:
            if (GVARS_PLAYER_LANDED != 0) {
                PLAYER_SUBSTATE = 2;
                D_8009C93F = 1;
                D_8009C942 = 1;
                PLAYER_STEP = 0;
                o->w08 = 0xC8;
                func_8005A9A4(0x4F, 0);
                o->state++;
            }
            break;
        case 1:
            if (--o->w08 <= 0) {
                D_8009C942 = 0;
                D_8009C93F = 0;
                D_800A60E4 = 0;
                PLAYER_STATE = 1;
                PLAYER_SUBSTATE = 0;
                PLAYER_STEP = 0;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        break;
    case 3:
        tunnelSlidePlayerStep(o);
        break;
    }
}
