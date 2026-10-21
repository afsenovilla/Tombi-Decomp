// FUNC 800ea148 772 I0NFO2
// MATCHING 800ea148 772
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
typedef struct { char p[0x2C]; unsigned short w2c, w2e; } P330;
extern P330 *D_8009C330;
extern void *D_800106B4, *D_800106B8, *D_800106BC, *D_800106C0, *D_800106C4, *D_800106C8, *D_800106CC, *D_800106D0;
extern void *D_800106D4, *D_800106D8, *D_800106DC, *D_800106E0, *D_800106E4, *D_800106E8, *D_800106EC;

void topdownPlayerSetDirAnimCached(TObj *o, int mode, int dir)
{
    char d = dir;
    if (D_8009C330->w2c == D_8009C330->w2e) return;
    D_8009C330->w2c = dir + mode * 8;
    switch ((short)mode) {
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
    D_8009C330->w2e = D_8009C330->w2c;
}
