// VERSION SLPM_80165 x00 800ED5FC 684
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerGroundAnimTransition (src/x001/playerGroundAnimTransition.c): the demo's walk / jog / run
// anims have other frame numbers (case labels read off the demo jump tables), no crouch-walk case; transition 33
// loads the anim D_80010A90 like transition 0. Each of the three steps calls AnimJump itself (cross-jumped).
#include "TOBJ.H"
extern char D_80010AFC[], D_80010A90[], D_80010B44[], D_80010BB8[], D_80010C3C[];
extern void AnimLoadDuration(TObj *);
extern void AnimJump(TObj *, int);

void func_800ED5FC(TObj *o, unsigned short k)
{
    int r = 0;

    switch (k) {
    case 0:
        o->anim = D_80010AFC;
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
        o->anim = D_80010B44;
        AnimJump(o, r);
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
        o->anim = D_80010BB8;
        AnimJump(o, r);
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
        o->anim = D_80010C3C;
        AnimJump(o, r);
        break;
    case 33:
        o->anim = D_80010A90;
        AnimLoadDuration(o);
        return;
    }
}
#endif
