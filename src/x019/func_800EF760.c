// FUNC 800ef760 800 X019
// MATCHING 800ef760 800
#include "TOBJ.H"
extern char D_80010748[];
extern char D_80010814[];
extern char D_80010888[];
extern char D_8001090C[];
extern char D_800111E0[];
extern void AnimLoadDuration(TObj *);
extern void AnimJump(TObj *, int);

void func_800EF760(TObj *o, unsigned short k)
{
    int r = 0;

    switch (k) {
    case 0:
        o->anim = D_80010748;
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
        o->anim = D_80010814;
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
        o->anim = D_80010888;
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
        o->anim = D_8001090C;
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
        o->anim = D_800111E0;
        break;
    default:
        return;
    }
    AnimJump(o, r);
}
