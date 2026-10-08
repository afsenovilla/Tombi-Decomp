// FUNC 800eff1c 400 X006
// MATCHING 800eff1c 400
#include "TOBJ.H"
extern unsigned char *DAT_8009c330;

int FUN_800eff1c(TObj *o)
{
    int r = 0;
    unsigned char t;
    if (o->bbe) {
        o->velY = 0;
        o->b9c = 0;
        t = o->step;
        if (!(t == 0x10 || t == 0x17 || t == 0x1b || t == 0x1e || t == 0x1f)) {
            if (o->bbe & 2) {
                if ((o->bbe & 1) ? (o->wb2 < -0x144) : (o->wb2 > 0x144))
                    r = 1;
            } else if (o->bbe & 4) {
                r = 2;
            } else if (o->bbe & 8) {
                r = 3;
            } else if (o->bbe & 0x10) {
                if ((o->bbe & 1) ? (o->wb2 < -0x144) : (o->wb2 > 0x144))
                    r = 4;
            } else if (o->bbe & 0x40) {
                r = 5;
            }
        }
        if ((o->bbe & 0x20) && o->step != 0x1f) {
            if (*DAT_8009c330 == 0 && ((unsigned char *)o)[0xc6] == 0) {
                DAT_8009c330[8] = o->animFrame & 1;
                o->step = 0x1f;
                o->state = 0;
            }
            r = 0;
        }
    }
    return r;
}
