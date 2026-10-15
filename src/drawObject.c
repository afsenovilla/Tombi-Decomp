// FUNC 800500c4 872 MAIN0
// MATCHING 800500c4 872
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "player.h"
extern unsigned short D_8009C960s;
int getObjectRenderKind(TObj *o);
void func_8011CEF0(TObj *o);
void func_8011D1BC(TObj *o);
void func_8011D4E0(TObj *o);
void func_8011DBF4(TObj *o);
void drawObjectSprite(TObj *o);
void drawObjectSpriteScaled(TObj *o);
void drawObjectSpriteRotated(TObj *o);
void drawObjectSpriteRotScaled(TObj *o);
void drawObjectSpriteQuad(TObj *o);
void func_800EAB58(TObj *o);
void func_800EC148(TObj *o);
void func_800EB77C(TObj *o);
void drawObjectSpriteRotatedFT4(TObj *o);
void func_8011EBC0(TObj *o);
void func_8011F240(TObj *o);
void func_8011B830(TObj *o);
void func_8011C00C(TObj *o);
void drawObjectSprtTinted(TObj *o);
void drawObjectModelNoRot(TObj *o);
void drawObjectModelRotated(TObj *o);
void drawObjectModelRotatedVtx(TObj *o);
void drawObjectModelRotScaled(TObj *o);
void drawObjectModelCameraFixed(TObj *o);
void drawObjectModelOwnMatrix(TObj *o);
void drawObjectModelOwnMatrixVtx(TObj *o);

void drawObject(TObj *o)
{
    if (o->visible == 0) return;
    if ((o->b0a & 0x10) && o->da0 == 0) return;
    if ((*(int*)&GAME.selectedArea) == 9 && GAME.selectedPlane != 0x20 && GVARS.b3a == 1 &&
        o->b.p.whole < 0xb6d && (o->category & 0x7f) != 8) return;
    if (D_8009C960s == 10 && (GAME.selectedSection == 0 || GAME.selectedSection == 4) && GAME.selectedPlane != 0 &&
        (unsigned short)(o->b.p.whole + 9) < 0x13 && (o->category & 0x7f) != 8) return;
    if ((*(int*)&GAME.selectedArea) == 6) {
        switch (o->b0a) {
        case 0: func_8011CEF0(o); break;
        case 1: func_8011D1BC(o); break;
        case 2: func_8011D4E0(o); break;
        case 3: func_8011DBF4(o); break;
        case 0x10: goto c10;
        case 0x11: goto c11;
        case 0x12: goto c12;
        case 0x13: goto c13;
        case 0x14: goto c14;
        case 0x15: goto c15;
        case 0x16: goto c16;
        }
        return;
    }
    switch (getObjectRenderKind(o)) {
    case 0: drawObjectSprite(o); break;
    case 1: drawObjectSpriteScaled(o); break;
    case 2: drawObjectSpriteRotated(o); break;
    case 3: drawObjectSpriteRotScaled(o); break;
    case 4: drawObjectSpriteQuad(o); break;
    case 5:
        switch (PLAYER.obj.type) {
        case 1: func_800EAB58(o); break;
        case 2: func_800EC148(o); break;
        case 3: func_800EB77C(o); break;
        }
        break;
    case 6: drawObjectSpriteRotatedFT4(o); break;
    case 7: func_8011EBC0(o); break;
    case 8: func_8011F240(o); break;
    case 9: func_8011B830(o); break;
    case 10: func_8011C00C(o); break;
    case 13: drawObjectSprtTinted(o); break;
    case 0x10: c10: drawObjectModelNoRot(o); break;
    case 0x11: c11: drawObjectModelRotated(o); break;
    case 0x12: c12: drawObjectModelRotatedVtx(o); break;
    case 0x13: c13: drawObjectModelRotScaled(o); break;
    case 0x14: c14: drawObjectModelCameraFixed(o); break;
    case 0x15: c15: drawObjectModelOwnMatrix(o); break;
    case 0x16: c16: drawObjectModelOwnMatrixVtx(o); break;
    }
}
