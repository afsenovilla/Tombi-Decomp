// FUNC 8011ac58 320 X000
// MATCHING 8011ac58 320
#include "TOBJ.H"
extern unsigned char DAT_800a603c[];
extern unsigned char PLAYER_SUBSTATE, PLAYER_STEP, DAT_8009c93f, DAT_800a60a1;
extern unsigned char DAT_8009cdae;
extern unsigned char PLAYER_STATE;
extern short DAT_800a6066, PLAYER_Y;

void a00BackdropSpritesStep4(TObj *o)
{
    switch (o->step) {
    case 1:
        switch (o->state) {
        case 0:
            o->timer = 0xf0;
            o->state++;
        case 1:
            o->d34++;
            if (--o->timer == 0) {
                o->step = 2;
                o->state = 0;
                if (DAT_800a603c[0] != 1) {
                    DAT_800a6066 = 0;
                    DAT_800a603c[0] = 1;
                    PLAYER_SUBSTATE = 0;
                    PLAYER_STEP = 0;
                    DAT_8009c93f = 0;
                }
            }
        }
        break;
    case 5:
        if (DAT_8009cdae == 0xff && PLAYER_Y > -200 && DAT_800a60a1 != 0 && PLAYER_STATE != 1) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 0: case 2: case 3: case 4:
        break;
    case 6:
        o->d34 = 0x54;
        break;
    }
}
