// FUNC 80050ea4 484 MAIN0
// MATCHING 80050ea4 484
typedef struct { char p0[1]; unsigned char vis; char p1[8]; unsigned char k; } O;
extern short G1c6, G24e, G244;
extern unsigned short G244u;
extern O **G25c, **G218;
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
        G24e = G244u;
        G25c = G218;
        if (G244u != 0) {
            do {
                o = *G218++;
                G244u = G244u - 1;
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
