// FUNC 8011cc8c 408 X003
// MATCHING 8011cc8c 408
#include "TOBJ.H"
extern TObj PLAYER;
extern void *D_80139870[];
extern void playerLauncherWait(TObj *), playerLauncherAim(TObj *), playerLauncherGrab(TObj *), playerLauncherSwing(TObj *);
extern void playerLauncherThrow(TObj *), playerLauncherScriptedPull(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);

void stepPlayerLauncherScripted(TObj *o)
{
    switch (o->step) {
    case 0:
        playerLauncherWait(o);
        break;
    case 1:
        playerLauncherAim(o);
        break;
    case 2:
        playerLauncherGrab(o);
        break;
    case 3:
        playerLauncherSwing(o);
        break;
    case 4:
        playerLauncherThrow(o);
        break;
    case 5:
    case 6:
        break;
    case 7:
        switch (o->state) {
        case 0:
            break;
        case 1:
            if (AnimAdvance(o)) {
                { TObj *pl = &PLAYER; pl->y.p.whole--; }
                o->step = 3;
                o->state = 0;
                o->wac = 3;
                o->y.p.whole -= 0x30;
                o->anim = D_80139870[0];
                AnimLoadDuration(o);
            }
            break;
        }
        break;
    case 8:
        playerLauncherScriptedPull(o);
        break;
    }
    if (o->w74 & 2) {
        if (o->w74 & 1) o->d8c = (o->d38 + 0x80) & 0xff;
        else o->d8c = o->d38;
    } else {
        if ((unsigned char)(o->d38 - 0x40) < 0x80) {
            o->animFrame = 1;
            o->d8c = (o->d38 + 0x80) & 0xff;
        } else {
            o->animFrame = 0;
            o->d8c = o->d38;
        }
    }
}
