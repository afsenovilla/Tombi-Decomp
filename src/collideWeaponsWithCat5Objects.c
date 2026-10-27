// FUNC 80045d98 360 MAIN0
// MATCHING 80045d98 360
typedef struct { unsigned char act, p1, type; char p[0xa2]; unsigned char ba5; } O;
extern short ONSCREEN_OBJECT_COUNT, CAT5_OBJECT_COUNT, G19e;
extern unsigned short G19eu;
extern O **ONSCREEN_OBJECT_LIST;
extern unsigned char **CAT5_OBJECT_LIST;
extern void weaponHitCat5Object(O *, unsigned char *);

void collideWeaponsWithCat5Objects(void)
{
    short n = ONSCREEN_OBJECT_COUNT;
    O **pp = ONSCREEN_OBJECT_LIST;
    O *o;
    unsigned char **q;
    unsigned char *b;
    int s2;
    unsigned short v;
    if (CAT5_OBJECT_COUNT != 0 && n != 0) {
        do {
            o = *pp++;
            n--;
            if ((o->act & 1) && o->ba5 != 0) {
                q = CAT5_OBJECT_LIST;
                s2 = (unsigned char)(o->type - 5) < 3;
                v = CAT5_OBJECT_COUNT;
                G19e = v;
                if (v != 0) do {
                    b = *q;
                    G19eu--;
                    q++;
                    switch (s2) {
                    case 0:
                        if ((b[0] & 1) && (b[3] == 0 || b[3] == 2))
                            weaponHitCat5Object(o, b);
                        break;
                    case 1:
                        if (b[0] != 5 && (b[0] & 3))
                            weaponHitCat5Object(o, b);
                        break;
                    }
                } while (G19e != 0);
            }
        } while (n != 0);
    }
}
