// FUNC 80055174 1188 MAIN0
// MATCHING 80055174 1188
#include "TOBJ.H"

extern short DAT_1f8001c6;
extern short DAT_1f800252, DAT_1f80024a;
extern TObj **DAT_1f800220, **DAT_1f800264;
extern unsigned char DAT_800a603a;

int getObjectRenderKind(TObj *o);
void drawObjectSprite(TObj *o);
void drawObjectSpriteRotated(TObj *o);
void FUN_80055618(TObj *o);
void FUN_80123e04(TObj *o);
void drawObjectSpriteQuad(TObj *o);
void FUN_80055cb8(TObj *o);
void FUN_80052190(TObj *o);
void drawObjectSpriteScaled(TObj *o);
void drawMessageBox(TObj *o);
void drawObjectSpriteRotScaled(TObj *o);
void drawMessageBoxFrame(TObj *o);
void drawObjectSprtTinted(TObj *o);
void FUN_800eab58(TObj *o);
void FUN_800ec148(TObj *o);
void FUN_800eb77c(TObj *o);
void FUN_80056e4c(TObj *o);
void drawObjectSprtWithMode(TObj *o);
void FUN_80124780(TObj *o);
void FUN_8011c00c(TObj *o);
void FUN_800ecd88(TObj *o);
void FUN_801189ec(TObj *o);
void FUN_80118ddc(TObj *o);
void drawObjectModelRotated(TObj *o);

void drawCategory3Objects(void)
{
    int n;
    TObj **p;
    TObj *o;

    if (DAT_1f8001c6 != 0) {
        n = DAT_1f800252;
        p = DAT_1f800264;
        while (n != 0) {
            o = *p++;
            n--;
            switch (getObjectRenderKind(o)) {
            case 0: drawObjectSprite(o); break;
            case 2: drawObjectSpriteRotated(o); break;
            case 4: FUN_80055618(o); break;
            case 5: FUN_80123e04(o); break;
            case 6: drawObjectSpriteQuad(o); break;
            case 7: FUN_80055cb8(o); break;
            case 8: FUN_80052190(o); break;
            case 1: case 9: drawObjectSpriteScaled(o); break;
            case 10: drawMessageBox(o); break;
            case 3: case 11: drawObjectSpriteRotScaled(o); break;
            case 12: drawMessageBoxFrame(o); break;
            case 13:
                switch (DAT_800a603a) {
                case 0: drawObjectSprtTinted(o); break;
                case 1: FUN_800eab58(o); break;
                case 2: FUN_800ec148(o); break;
                case 3: FUN_800eb77c(o); break;
                }
            case 14: FUN_80056e4c(o); break;
            case 16: drawObjectSprtWithMode(o); break;
            case 17: FUN_80124780(o); break;
            case 18: FUN_8011c00c(o); break;
            case 19: FUN_800ecd88(o); break;
            case 20: FUN_801189ec(o); break;
            case 21: FUN_80118ddc(o); break;
            case 22: drawObjectModelRotated(o); break;
            }
        }
    } else {
        DAT_1f800252 = DAT_1f80024a;
        DAT_1f800264 = DAT_1f800220;
        while (DAT_1f80024a != 0) {
            o = *DAT_1f800220++;
            DAT_1f80024a--;
            if (o->visible) {
                switch (getObjectRenderKind(o)) {
                case 0: drawObjectSprite(o); break;
                case 2: drawObjectSpriteRotated(o); break;
                case 4: FUN_80055618(o); break;
                case 5: FUN_80123e04(o); break;
                case 6: drawObjectSpriteQuad(o); break;
                case 7: FUN_80055cb8(o); break;
                case 8: FUN_80052190(o); break;
                case 1: case 9: drawObjectSpriteScaled(o); break;
                case 10: drawMessageBox(o); break;
                case 3: case 11: drawObjectSpriteRotScaled(o); break;
                case 12: drawMessageBoxFrame(o); break;
                case 13:
                    switch (DAT_800a603a) {
                    case 0: drawObjectSprtTinted(o); break;
                    case 1: FUN_800eab58(o); break;
                    case 2: FUN_800ec148(o); break;
                    case 3: FUN_800eb77c(o); break;
                    }
                    break;
                case 14: FUN_80056e4c(o); break;
                case 16: drawObjectSprtWithMode(o); break;
                case 17: FUN_80124780(o); break;
                case 18: FUN_8011c00c(o); break;
                case 19: FUN_800ecd88(o); break;
                case 20: FUN_801189ec(o); break;
                case 21: FUN_80118ddc(o); break;
                case 22: drawObjectModelRotated(o); break;
                }
            }
        }
    }
}
