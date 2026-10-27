// FUNC 80054f54 544 MAIN0
// MATCHING 80054f54 544
#include "TOBJ.H"
extern short DAT_1f8001c6;
extern short CAT2_OBJECT_COUNT_SAVED;
extern TObj **CAT2_OBJECT_LIST_SAVED;
extern unsigned short DAT_1f800246;
extern short CAT2_OBJECT_COUNT;
extern TObj **CAT2_OBJECT_LIST;
extern unsigned char PLAYER_TYPE;
extern void drawObjectSprtTinted(TObj *);
extern void FUN_800eab58(TObj *);
extern void FUN_800ec148(TObj *);
extern void FUN_800eb77c(TObj *);
extern void drawObject(TObj *);

void drawCategory2Objects(void)
{
    int n;
    TObj **p;
    TObj *o;
    if (DAT_1f8001c6 != 0) {
        n = CAT2_OBJECT_COUNT_SAVED;
        p = CAT2_OBJECT_LIST_SAVED;
        while (n != 0) {
            o = *p++;
            n--;
            if (o->b0a == 0xd) {
                switch (PLAYER_TYPE) {
                case 0: drawObjectSprtTinted(o); break;
                case 1: FUN_800eab58(o); break;
                case 2: FUN_800ec148(o); break;
                case 3: FUN_800eb77c(o); break;
                }
            } else {
                drawObject(o);
            }
        }
    } else {
        CAT2_OBJECT_COUNT_SAVED = DAT_1f800246;
        CAT2_OBJECT_LIST_SAVED = CAT2_OBJECT_LIST;
        if (DAT_1f800246 == 0)
            return;
        do {
            o = *CAT2_OBJECT_LIST++;
            DAT_1f800246--;
            if (o->visible) {
                if (o->b0a == 0xd) {
                    switch (PLAYER_TYPE) {
                    case 0: drawObjectSprtTinted(o); break;
                    case 1: FUN_800eab58(o); break;
                    case 2: FUN_800ec148(o); break;
                    case 3: FUN_800eb77c(o); break;
                    }
                } else {
                    drawObject(o);
                }
            }
        } while (CAT2_OBJECT_COUNT != 0);
    }
}
