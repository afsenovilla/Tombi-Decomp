// FUNC 8011899c 548 X000
// MATCHING 8011899c 548
#include "TOBJ.H"
extern unsigned char GAME_AREA00_INTRO_PHASE;
extern unsigned char DAT_8009cda6;
extern void a00FogCloudWobble(TObj *);
extern void FUN_80018ca4(TObj *);

void a00FogCloudClearStep(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GAME_AREA00_INTRO_PHASE == 3 || DAT_8009cda6 == 0xff) {
            o->b04 = 3;
            break;
        }
        o->_pad0e[0] = 1;
        o->b0b = 1;
        o->b0f = 100;
        o->d30 = o->h->raw;
        o->d34 = o->y.raw;
        o->step++;
    case 1:
        switch (GAME_AREA00_INTRO_PHASE) {
        case 0:
            break;
        case 1:
            o->step = 2;
            break;
        case 2:
            o->step = 3;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        break;
    case 2:
        if (o->h->p.whole >= 0x4d9)
            o->h->raw -= 0x18000;
        a00FogCloudWobble(o);
        o->visible = 1;
        FUN_80018ca4(o);
        switch (GAME_AREA00_INTRO_PHASE) {
        case 0:
            break;
        case 2:
            o->step = 3;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        if (o->h->raw >= o->d30) {
            o->h->raw = o->d30;
            o->step = 0;
            o->y.raw = o->d34;
            break;
        }
        switch (GAME_AREA00_INTRO_PHASE) {
        case 0:
            break;
        case 1:
            o->step = 2;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        o->h->raw += 0x18000;
        a00FogCloudWobble(o);
        o->visible = 1;
        FUN_80018ca4(o);
        break;
    }
}
