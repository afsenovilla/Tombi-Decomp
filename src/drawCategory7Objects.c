// FUNC 80057edc 652 MAIN0
// MATCHING 80057edc 652
#include "TOBJ.H"
extern short DAT_1f8001c6;
extern short CAT7_OBJECT_COUNT_SAVED;
extern TObj **DAT_1f800270;
extern short CAT7_OBJECT_COUNT;
extern TObj **DAT_1f80022c;
extern unsigned char PLAYER_TYPE;
extern unsigned short GAME[];
extern void drawObjectSprtWithMode(TObj *);
extern void drawScreenSpriteObject(TObj *);
extern void drawLetterboxOverlay(TObj *);
extern void FUN_800eb044(TObj *);
extern void spawnWeaponTrailObjects(TObj *);
extern void FUN_8011e778(TObj *);
extern void FUN_80116fb8(TObj *);

void drawCategory7Objects(void)
{
    TObj *o;
    TObj **p;
    int n;

    if (DAT_1f8001c6 != 0) {
        n = CAT7_OBJECT_COUNT_SAVED;
        p = DAT_1f800270;
        while (n != 0) {
            o = *p++;
            n--;
            switch (o->b0a) {
            case 0:
                drawObjectSprtWithMode(o);
                break;
            case 1:
                drawScreenSpriteObject(o);
                break;
            case 2:
                drawLetterboxOverlay(o);
                break;
            case 3:
                if (PLAYER_TYPE & 2)
                    spawnWeaponTrailObjects(o);
                else
                    FUN_800eb044(o);
                break;
            case 4:
                if (GAME[0] == 4)
                    FUN_8011e778(o);
                if (GAME[0] == 0xc)
                    FUN_80116fb8(o);
                break;
            }
        }
    } else {
        CAT7_OBJECT_COUNT_SAVED = CAT7_OBJECT_COUNT;
        DAT_1f800270 = DAT_1f80022c;
        while (CAT7_OBJECT_COUNT != 0) {
            o = *DAT_1f80022c++;
            CAT7_OBJECT_COUNT--;
            switch (o->b0a) {
            case 0:
                drawObjectSprtWithMode(o);
                break;
            case 1:
                drawScreenSpriteObject(o);
                break;
            case 2:
                drawLetterboxOverlay(o);
                break;
            case 3:
                if (PLAYER_TYPE & 2)
                    spawnWeaponTrailObjects(o);
                else
                    FUN_800eb044(o);
                break;
            case 4:
                if (GAME[0] == 4)
                    FUN_8011e778(o);
                if (GAME[0] == 0xc)
                    FUN_80116fb8(o);
                break;
            }
        }
    }
}
