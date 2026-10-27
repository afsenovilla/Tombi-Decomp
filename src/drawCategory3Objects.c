// FUNC 80055174 1188 MAIN0
// MATCHING 80055174 1188
#include "TOBJ.H"

extern short DAT_1f8001c6;
extern short CAT3_OBJECT_COUNT_SAVED, CAT3_OBJECT_COUNT;
extern TObj **DAT_1f800220, **DAT_1f800264;
extern unsigned char PLAYER_TYPE;

int getObjectRenderKind(TObj *o);
void drawObjectSprite(TObj *o);
void drawObjectSpriteRotated(TObj *o);
void drawObjectMultiPartSprite3D(TObj *o);
void drawObjectProjectedSpriteB(TObj *o);
void drawObjectSpriteQuad(TObj *o);
void drawObjectSpriteParts(TObj *o);
void drawObjectSpritePartsRotated3D(TObj *o);
void drawObjectSpriteScaled(TObj *o);
void drawMessageBox(TObj *o);
void drawObjectSpriteRotScaled(TObj *o);
void drawMessageBoxFrame(TObj *o);
void drawObjectSprtTinted(TObj *o);
void FUN_800eab58(TObj *o);
void FUN_800ec148(TObj *o);
void FUN_800eb77c(TObj *o);
void drawObjectSpriteScaled3D(TObj *o);
void drawObjectSprtWithMode(TObj *o);
void drawObjectPolyFT4Strip(TObj *o);
void FUN_8011c00c(TObj *o);
void drawObjectModelKind19(TObj *o);
void FUN_801189ec(TObj *o);
void FUN_80118ddc(TObj *o);
void drawObjectModelRotated(TObj *o);

void drawCategory3Objects(void)
{
    int n;
    TObj **p;
    TObj *o;

    if (DAT_1f8001c6 != 0) {
        n = CAT3_OBJECT_COUNT_SAVED;
        p = DAT_1f800264;
        while (n != 0) {
            o = *p++;
            n--;
            switch (getObjectRenderKind(o)) {
            case 0: drawObjectSprite(o); break;
            case 2: drawObjectSpriteRotated(o); break;
            case 4: drawObjectMultiPartSprite3D(o); break;
            case 5: drawObjectProjectedSpriteB(o); break;
            case 6: drawObjectSpriteQuad(o); break;
            case 7: drawObjectSpriteParts(o); break;
            case 8: drawObjectSpritePartsRotated3D(o); break;
            case 1: case 9: drawObjectSpriteScaled(o); break;
            case 10: drawMessageBox(o); break;
            case 3: case 11: drawObjectSpriteRotScaled(o); break;
            case 12: drawMessageBoxFrame(o); break;
            case 13:
                switch (PLAYER_TYPE) {
                case 0: drawObjectSprtTinted(o); break;
                case 1: FUN_800eab58(o); break;
                case 2: FUN_800ec148(o); break;
                case 3: FUN_800eb77c(o); break;
                }
            case 14: drawObjectSpriteScaled3D(o); break;
            case 16: drawObjectSprtWithMode(o); break;
            case 17: drawObjectPolyFT4Strip(o); break;
            case 18: FUN_8011c00c(o); break;
            case 19: drawObjectModelKind19(o); break;
            case 20: FUN_801189ec(o); break;
            case 21: FUN_80118ddc(o); break;
            case 22: drawObjectModelRotated(o); break;
            }
        }
    } else {
        CAT3_OBJECT_COUNT_SAVED = CAT3_OBJECT_COUNT;
        DAT_1f800264 = DAT_1f800220;
        while (CAT3_OBJECT_COUNT != 0) {
            o = *DAT_1f800220++;
            CAT3_OBJECT_COUNT--;
            if (o->visible) {
                switch (getObjectRenderKind(o)) {
                case 0: drawObjectSprite(o); break;
                case 2: drawObjectSpriteRotated(o); break;
                case 4: drawObjectMultiPartSprite3D(o); break;
                case 5: drawObjectProjectedSpriteB(o); break;
                case 6: drawObjectSpriteQuad(o); break;
                case 7: drawObjectSpriteParts(o); break;
                case 8: drawObjectSpritePartsRotated3D(o); break;
                case 1: case 9: drawObjectSpriteScaled(o); break;
                case 10: drawMessageBox(o); break;
                case 3: case 11: drawObjectSpriteRotScaled(o); break;
                case 12: drawMessageBoxFrame(o); break;
                case 13:
                    switch (PLAYER_TYPE) {
                    case 0: drawObjectSprtTinted(o); break;
                    case 1: FUN_800eab58(o); break;
                    case 2: FUN_800ec148(o); break;
                    case 3: FUN_800eb77c(o); break;
                    }
                    break;
                case 14: drawObjectSpriteScaled3D(o); break;
                case 16: drawObjectSprtWithMode(o); break;
                case 17: drawObjectPolyFT4Strip(o); break;
                case 18: FUN_8011c00c(o); break;
                case 19: drawObjectModelKind19(o); break;
                case 20: FUN_801189ec(o); break;
                case 21: FUN_80118ddc(o); break;
                case 22: drawObjectModelRotated(o); break;
                }
            }
        }
    }
}
