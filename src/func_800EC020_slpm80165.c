// VERSION SLPM_80165 x00 800EC020 100
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of PlayerSetAnimIfChanged (src/x011/PlayerSetAnimIfChanged.c): one store of the new animation, and the
// frame reset takes only the object.
#include "TOBJ.H"
extern TObj *PLAYER_EXT;
extern void func_800EDA04(TObj *);
extern void func_80020554(TObj *);

void func_800EC020(TObj *o, unsigned short anim)
{
    TObj *p = PLAYER_EXT;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        func_800EDA04(o);
        func_80020554(o);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
}
#endif
