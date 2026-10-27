// FUNC 80050ea4 484 MAIN0
// MATCHING 80050ea4 484
// VERSION SLPM_80165 slpm80165 800408B4 428
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80050EA4 (FUN_80050ea4.c): no kind 0xd, no visibility test.
typedef struct { char p0[1]; unsigned char vis; char p1[8]; unsigned char k; } O;
extern short G1c6, G24e, G244;
extern unsigned short ONSCREEN_OBJECT_COUNT;
extern O **G25c, **ONSCREEN_OBJECT_LIST;
extern void FUN_80051088(O *), FUN_800532fc(O *), FUN_80054868(O *), FUN_80057470(O *);

void FUN_80050ea4(void)
{
    int n;
    O **pp;
    O *o;
    if (G1c6 != 0) {
        n = G24e;
        pp = G25c;
        if (n != 0) {
            do {
                o = *pp++;
                n--;
                switch (o->k) {
                case 0: FUN_80051088(o); break;
                case 2: FUN_800532fc(o); break;
                case 0x11: FUN_80057470(o); break;
                }
            } while (n != 0);
        }
    } else {
        G24e = ONSCREEN_OBJECT_COUNT;
        G25c = ONSCREEN_OBJECT_LIST;
        if (ONSCREEN_OBJECT_COUNT != 0) {
            do {
                o = *ONSCREEN_OBJECT_LIST++;
                ONSCREEN_OBJECT_COUNT = ONSCREEN_OBJECT_COUNT - 1;
                switch (o->k) {
                case 0: FUN_80051088(o); break;
                case 2: FUN_800532fc(o); break;
                case 0x11: FUN_80057470(o); break;
                }
            } while (G244 != 0);
        }
    }
}
#else
typedef struct { char p0[1]; unsigned char vis; char p1[8]; unsigned char k; } O;
extern short G1c6, G24e, G244;
extern unsigned short ONSCREEN_OBJECT_COUNT;
extern O **G25c, **ONSCREEN_OBJECT_LIST;
extern void drawWeaponObject(O *), drawObjectSpriteRotated(O *), drawObjectSprtTinted(O *), drawObjectModelRotated(O *);

void drawCategory1Objects(void)
{
    int n;
    O **pp;
    O *o;
    if (G1c6 != 0) {
        n = G24e;
        pp = G25c;
        if (n != 0) {
            do {
                o = *pp++;
                n--;
                switch (o->k) {
                case 0: drawWeaponObject(o); break;
                case 2: drawObjectSpriteRotated(o); break;
                case 0xd: drawObjectSprtTinted(o); break;
                case 0x11: drawObjectModelRotated(o); break;
                }
            } while (n != 0);
        }
    } else {
        G24e = ONSCREEN_OBJECT_COUNT;
        G25c = ONSCREEN_OBJECT_LIST;
        if (ONSCREEN_OBJECT_COUNT != 0) {
            do {
                o = *ONSCREEN_OBJECT_LIST++;
                ONSCREEN_OBJECT_COUNT = ONSCREEN_OBJECT_COUNT - 1;
                if (o->vis != 0) {
                    switch (o->k) {
                    case 0: drawWeaponObject(o); break;
                    case 2: drawObjectSpriteRotated(o); break;
                    case 0xd: drawObjectSprtTinted(o); break;
                    case 0x11: drawObjectModelRotated(o); break;
                    }
                }
            } while (G244 != 0);
        }
    }
}
#endif
