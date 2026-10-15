// FUNC 80045d98 360 MAIN0
// MATCHING 80045d98 360
typedef struct { unsigned char act, p1, type; char p[0xa2]; unsigned char ba5; } O;
extern short G244, G248, G19e;
extern unsigned short G19eu;
extern O **G218;
extern unsigned char **G228;
extern void weaponHitCat5Object(O *, unsigned char *);

void collideWeaponsWithCat5Objects(void)
{
    short n = G244;
    O **pp = G218;
    O *o;
    unsigned char **q;
    unsigned char *b;
    int s2;
    unsigned short v;
    if (G248 != 0 && n != 0) {
        do {
            o = *pp++;
            n--;
            if ((o->act & 1) && o->ba5 != 0) {
                q = G228;
                s2 = (unsigned char)(o->type - 5) < 3;
                v = G248;
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
