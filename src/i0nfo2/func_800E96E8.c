// FUNC 800e96e8 712 I0NFO2
// MATCHING 800e96e8 712
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void *D_800106B4, *D_800106B8, *D_800106BC, *D_800106C0, *D_800106C4, *D_800106C8, *D_800106CC, *D_800106D0;
extern void *D_800106D4, *D_800106D8, *D_800106DC, *D_800106E0, *D_800106E4, *D_800106E8, *D_800106EC;

void func_800E96E8(TObj *o, short mode, int dir)
{
    char d = dir;
    switch (mode) {
    case 0:
        switch (dir &= 7) {
        case 0: o->anim = D_800106BC; break;
        case 1: o->anim = D_800106B8; break;
        case 2: o->anim = D_800106B4; break;
        case 3: o->anim = D_800106B8; break;
        case 4: o->anim = D_800106BC; break;
        case 6: o->anim = D_800106C4; break;
        case 5: case 7: o->anim = D_800106C0; break;
        }
        break;
    case 1:
        switch (dir &= 7) {
        case 0: o->anim = D_800106D0; break;
        case 1: o->anim = D_800106CC; break;
        case 2: o->anim = D_800106C8; break;
        case 3: o->anim = D_800106CC; break;
        case 4: o->anim = D_800106D0; break;
        case 5: o->anim = D_800106D4; break;
        case 6: o->anim = D_800106D8; break;
        case 7: o->anim = D_800106D4; break;
        }
        break;
    case 2:
        switch (dir &= 7) {
        case 0: o->anim = D_800106E4; break;
        case 1: o->anim = D_800106E0; break;
        case 2: o->anim = D_800106DC; break;
        case 3: o->anim = D_800106E0; break;
        case 4: o->anim = D_800106E4; break;
        case 5: o->anim = D_800106E8; break;
        case 6: o->anim = D_800106EC; break;
        case 7: o->anim = D_800106E8; break;
        }
        break;
    default:
        switch (d & 7) {
        case 0: o->anim = D_800106BC; break;
        case 1: o->anim = D_800106B8; break;
        case 2: o->anim = D_800106B4; break;
        case 3: o->anim = D_800106B8; break;
        case 4: o->anim = D_800106BC; break;
        case 6: o->anim = D_800106C4; break;
        case 5: case 7: o->anim = D_800106C0; break;
        }
        break;
    }
    func_8001FE6C(o);
}
