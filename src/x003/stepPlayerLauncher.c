// FUNC 8011ca70 540 X003
// MATCHING 8011ca70 540
#include "TOBJ.H"

extern void *D_80139878[];
extern unsigned short PLAYER_Y;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void playerLauncherWait(TObj *);
extern void playerLauncherAim(TObj *);
extern void playerLauncherGrab(TObj *);
extern void playerLauncherSwing(TObj *);
extern void playerLauncherThrow(TObj *);
extern void playerLauncherPullPlayer(TObj *);

void stepPlayerLauncher(TObj *o)
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
        switch (o->state) {
        case 0:
            o->timer = 15;
            o->d38 = 0x40;
            o->wac = 5;
            o->w74 = 0;
            o->b0a = 0;
            o->y.p.whole += 0x30;
            o->state++;
            o->anim = D_80139878[0];
            AnimLoadDuration(o);
            break;
        case 1:
            if (o->timer != 0) {
                o->timer--;
            } else if (AnimAdvance(o)) {
                o->state++;
                PLAYER_Y++;
            }
            break;
        case 2:
            o->state = 0;
            o->step++;
            break;
        }
        break;
    case 6:
        playerLauncherPullPlayer(o);
        break;
    }
    if (o->w74 & 2) {
        if (o->w74 & 1)
            o->d8c = (o->d38 + 0x80) & 0xff;
        else
            o->d8c = o->d38;
    } else if ((unsigned)((o->d38 - 0x40) & 0xff) < 0x80) {
        o->animFrame = 1;
        o->d8c = (o->d38 + 0x80) & 0xff;
    } else {
        o->animFrame = 0;
        o->d8c = o->d38;
    }
}
