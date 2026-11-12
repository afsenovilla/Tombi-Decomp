// FUNC 800ef760 800 X000
// MATCHING 800ef760 800
// VERSION SLPS_01144 x00 800EF8CC 800
// VERSION BETA_980203 x00 800F4288 800
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern char PLAYER_IDLE_ANIM[];
extern char PLAYER_WALK_ANIM[];
extern char PLAYER_JOG_ANIM[];
extern char PLAYER_RUN_ANIM[];
extern char PLAYER_CROUCH_WALK_ANIM[];
extern void AnimLoadDuration(TObj *);
extern void AnimJump(TObj *, int);

void func_800EF8CC(TObj *o, unsigned short k)
{
    int r = 0;

    switch (k) {
    case 0:
        o->anim = PLAYER_IDLE_ANIM;
        AnimLoadDuration(o);
        return;
    case 1:
        switch (*(unsigned short *)o->anim) {
        case 8:
        case 22:
        case 38:
            r = 0;
            break;
        case 9:
        case 23:
            r = 1;
            break;
        case 10:
        case 24:
            r = 2;
            break;
        case 11:
        case 25:
            r = 3;
            break;
        case 12:
        case 26:
        case 27:
            r = 4;
            break;
        case 13:
        case 28:
            r = 5;
            break;
        case 14:
        case 29:
            r = 6;
            break;
        case 15:
        case 30:
            r = 7;
            break;
        case 16:
        case 31:
            r = 8;
            break;
        case 17:
        case 32:
            r = 9;
            break;
        case 18:
        case 33:
            r = 10;
            break;
        case 19:
        case 34:
        case 35:
            r = 11;
            break;
        case 20:
        case 36:
            r = 12;
            break;
        case 21:
        case 37:
            r = 13;
            break;
        }
        o->anim = PLAYER_WALK_ANIM;
        break;
    case 2:
        switch (*(unsigned short *)o->anim) {
        case 22:
        case 38:
        case 54:
            r = 0;
            break;
        case 9:
        case 23:
        case 39:
            r = 1;
            break;
        case 10:
        case 24:
        case 40:
            r = 2;
            break;
        case 11:
        case 25:
        case 41:
            r = 3;
            break;
        case 26:
        case 42:
            r = 4;
            break;
        case 12:
        case 27:
        case 43:
            r = 5;
            break;
        case 13:
        case 28:
        case 44:
            r = 6;
            break;
        case 14:
        case 29:
        case 45:
            r = 7;
            break;
        case 15:
        case 30:
        case 46:
            r = 8;
            break;
        case 16:
        case 31:
        case 47:
            r = 9;
            break;
        case 17:
        case 32:
        case 48:
            r = 10;
            break;
        case 18:
        case 33:
        case 49:
            r = 11;
            break;
        case 34:
        case 50:
            r = 12;
            break;
        case 19:
        case 35:
        case 51:
            r = 13;
            break;
        case 20:
        case 36:
        case 52:
            r = 14;
            break;
        case 21:
        case 37:
        case 53:
            r = 15;
            break;
        }
        o->anim = PLAYER_JOG_ANIM;
        break;
    case 3:
        switch (*(unsigned short *)o->anim) {
        case 38:
        case 54:
            r = 0;
            break;
        case 23:
        case 39:
            r = 1;
            break;
        case 24:
        case 40:
            r = 2;
            break;
        case 25:
        case 41:
            r = 3;
            break;
        case 26:
        case 42:
            r = 4;
            break;
        case 27:
        case 43:
            r = 5;
            break;
        case 28:
        case 44:
            r = 6;
            break;
        case 29:
        case 45:
            r = 7;
            break;
        case 30:
        case 46:
            r = 8;
            break;
        case 31:
        case 47:
            r = 9;
            break;
        case 32:
        case 48:
            r = 10;
            break;
        case 33:
        case 49:
            r = 11;
            break;
        case 34:
        case 50:
            r = 12;
            break;
        case 35:
        case 51:
            r = 13;
            break;
        case 36:
        case 52:
            r = 14;
            break;
        case 37:
        case 53:
            r = 15;
            break;
        }
        o->anim = PLAYER_RUN_ANIM;
        break;
    case 33:
        switch (*(unsigned short *)o->anim) {
        case 273:
            r = 1;
            break;
        case 274:
            r = 2;
            break;
        case 275:
            r = 3;
            break;
        case 276:
            r = 4;
            break;
        case 277:
            r = 5;
            break;
        case 278:
            r = 6;
            break;
        case 279:
            r = 7;
            break;
        case 280:
            r = 0;
            break;
        }
        o->anim = PLAYER_CROUCH_WALK_ANIM;
        break;
    default:
        return;
    }
    AnimJump(o, r);
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL x000 800EF760 (playerGroundAnimTransition.c): SLPS_01144 branch of src/playerGroundAnimTransition.c (0.990 masked) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
extern char PLAYER_IDLE_ANIM[];
extern char PLAYER_WALK_ANIM[];
extern char PLAYER_JOG_ANIM[];
extern char PLAYER_RUN_ANIM[];
extern char PLAYER_CROUCH_WALK_ANIM[];
extern void AnimLoadDuration(TObj *);
extern void AnimJump(TObj *, int);

void func_800EF8CC(TObj *o, unsigned short k)
{
    int r = 0;

    switch (k) {
    case 0:
        o->anim = PLAYER_IDLE_ANIM;
        AnimLoadDuration(o);
        return;
    case 1:
        switch (*(unsigned short *)o->anim) {
        case 8:
        case 38:
            r = 0;
            break;
        case 9:
            r = 1;
            break;
        case 10:
            r = 2;
            break;
        case 12:
            r = 3;
            break;
        case 13:
            r = 4;
            break;
        case 14:
            r = 5;
            break;
        case 15:
            r = 6;
            break;
        case 16:
            r = 7;
            break;
        case 17:
            r = 8;
            break;
        case 18:
            r = 9;
            break;
        case 19:
            r = 10;
            break;
        case 20:
            r = 11;
            break;
        case 21:
            r = 12;
            break;
        case 22:
            r = 13;
            break;
        }
        o->anim = PLAYER_WALK_ANIM;
        break;
    case 2:
        switch (*(unsigned short *)o->anim) {
        case 17:
        case 34:
            r = 0;
            break;
        case 18:
        case 35:
            r = 1;
            break;
        case 19:
        case 36:
            r = 2;
            break;
        case 20:
        case 37:
            r = 3;
            break;
        case 38:
            r = 4;
            break;
        case 21:
        case 39:
            r = 5;
            break;
        case 22:
        case 54:
            r = 6;
            break;
        case 24:
        case 25:
            r = 7;
            break;
        case 9:
        case 26:
            r = 8;
            break;
        case 10:
        case 27:
            r = 9;
            break;
        case 12:
        case 28:
            r = 10;
            break;
        case 13:
        case 29:
            r = 11;
            break;
        case 30:
            r = 12;
            break;
        case 14:
        case 31:
            r = 13;
            break;
        case 15:
        case 32:
            r = 14;
            break;
        case 16:
        case 33:
            r = 15;
            break;
        }
        o->anim = PLAYER_JOG_ANIM;
        break;
    case 3:
        switch (*(unsigned short *)o->anim) {
        case 32:
        case 54:
            r = 0;
            break;
        case 33:
        case 39:
            r = 1;
            break;
        case 34:
        case 40:
            r = 2;
            break;
        case 35:
        case 41:
            r = 3;
            break;
        case 36:
        case 42:
            r = 4;
            break;
        case 37:
        case 43:
            r = 5;
            break;
        case 38:
        case 44:
            r = 6;
            break;
        case 23:
        case 45:
            r = 7;
            break;
        case 24:
        case 46:
            r = 8;
            break;
        case 25:
        case 47:
            r = 9;
            break;
        case 26:
        case 48:
            r = 10;
            break;
        case 27:
        case 49:
            r = 11;
            break;
        case 28:
        case 50:
            r = 12;
            break;
        case 29:
        case 51:
            r = 13;
            break;
        case 30:
        case 52:
            r = 14;
            break;
        case 31:
        case 53:
            r = 15;
            break;
        }
        o->anim = PLAYER_RUN_ANIM;
        break;
    case 33:
        switch (*(unsigned short *)o->anim) {
        case 273:
            r = 1;
            break;
        case 274:
            r = 2;
            break;
        case 275:
            r = 3;
            break;
        case 276:
            r = 4;
            break;
        case 277:
            r = 5;
            break;
        case 278:
            r = 6;
            break;
        case 279:
            r = 7;
            break;
        case 280:
            r = 0;
            break;
        }
        o->anim = PLAYER_CROUCH_WALK_ANIM;
        break;
    default:
        return;
    }
    AnimJump(o, r);
}
#else
#include "TOBJ.H"
extern char PLAYER_IDLE_ANIM[];
extern char PLAYER_WALK_ANIM[];
extern char PLAYER_JOG_ANIM[];
extern char PLAYER_RUN_ANIM[];
extern char PLAYER_CROUCH_WALK_ANIM[];
extern void AnimLoadDuration(TObj *);
extern void AnimJump(TObj *, int);

void playerGroundAnimTransition(TObj *o, unsigned short k)
{
    int r = 0;

    switch (k) {
    case 0:
        o->anim = PLAYER_IDLE_ANIM;
        AnimLoadDuration(o);
        return;
    case 1:
        switch (*(unsigned short *)o->anim) {
        case 8:
        case 24:
            r = 0;
            break;
        case 9:
            r = 1;
            break;
        case 10:
            r = 2;
            break;
        case 12:
            r = 3;
            break;
        case 13:
            r = 4;
            break;
        case 14:
            r = 5;
            break;
        case 15:
            r = 6;
            break;
        case 16:
            r = 7;
            break;
        case 17:
            r = 8;
            break;
        case 18:
            r = 9;
            break;
        case 19:
            r = 10;
            break;
        case 20:
            r = 11;
            break;
        case 21:
            r = 12;
            break;
        case 22:
            r = 13;
            break;
        }
        o->anim = PLAYER_WALK_ANIM;
        break;
    case 2:
        switch (*(unsigned short *)o->anim) {
        case 17:
        case 34:
            r = 0;
            break;
        case 18:
        case 35:
            r = 1;
            break;
        case 19:
        case 36:
            r = 2;
            break;
        case 20:
        case 37:
            r = 3;
            break;
        case 38:
            r = 4;
            break;
        case 21:
        case 39:
            r = 5;
            break;
        case 22:
        case 40:
            r = 6;
            break;
        case 24:
        case 25:
            r = 7;
            break;
        case 9:
        case 26:
            r = 8;
            break;
        case 10:
        case 27:
            r = 9;
            break;
        case 12:
        case 28:
            r = 10;
            break;
        case 13:
        case 29:
            r = 11;
            break;
        case 30:
            r = 12;
            break;
        case 14:
        case 31:
            r = 13;
            break;
        case 15:
        case 32:
            r = 14;
            break;
        case 16:
        case 33:
            r = 15;
            break;
        }
        o->anim = PLAYER_JOG_ANIM;
        break;
    case 3:
        switch (*(unsigned short *)o->anim) {
        case 18:
        case 40:
            r = 0;
            break;
        case 19:
        case 25:
            r = 1;
            break;
        case 20:
        case 26:
            r = 2;
            break;
        case 21:
        case 27:
            r = 3;
            break;
        case 22:
        case 28:
            r = 4;
            break;
        case 23:
        case 29:
            r = 5;
            break;
        case 24:
        case 30:
            r = 6;
            break;
        case 9:
        case 31:
            r = 7;
            break;
        case 10:
        case 32:
            r = 8;
            break;
        case 11:
        case 33:
            r = 9;
            break;
        case 12:
        case 34:
            r = 10;
            break;
        case 13:
        case 35:
            r = 11;
            break;
        case 14:
        case 36:
            r = 12;
            break;
        case 15:
        case 37:
            r = 13;
            break;
        case 16:
        case 38:
            r = 14;
            break;
        case 17:
        case 39:
            r = 15;
            break;
        }
        o->anim = PLAYER_RUN_ANIM;
        break;
    case 33:
        switch (*(unsigned short *)o->anim) {
        case 223:
            r = 1;
            break;
        case 224:
            r = 2;
            break;
        case 225:
            r = 3;
            break;
        case 226:
            r = 4;
            break;
        case 227:
            r = 5;
            break;
        case 228:
            r = 6;
            break;
        case 229:
            r = 7;
            break;
        case 230:
            r = 0;
            break;
        }
        o->anim = PLAYER_CROUCH_WALK_ANIM;
        break;
    default:
        return;
    }
    AnimJump(o, r);
}
#endif
