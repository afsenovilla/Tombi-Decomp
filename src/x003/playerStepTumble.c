// FUNC 8011eae8 216 X003
// MATCHING 8011eae8 216
#include "TOBJ.H"
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void playerReleaseBittenAirborne(TObj *);

void playerStepTumble(TObj *o)
{
    int t, u;

    switch (o->state) {
    case 0:
        PlayerSetAnimIfChanged(o, 0x37);
        o->timer = 8;
        playerReleaseBittenAirborne(o);
        break;
    case 2:
        if (--o->timer > 0)
            break;
        PlayerSetAnimIfChanged(o, 0x10);
        o->state++;
        break;
    case 3:
        t = o->d8c;
        if (o->animFrame & 1)
            u = t + 0x10;
        else
            u = t - 0x10;
        o->d8c = u & 0xff;
        break;
    }
}
