// FUNC 800ec710 316 X014
// MATCHING 800ec710 316
typedef struct { unsigned char a; char p1; unsigned char c2, c3; char p4[8]; unsigned char cc; char pd[3]; int d10, d14, d18; char p1c[0x90 - 0x1c]; void *d90; } O;
extern O *func_80018448(void);

void FUN_800ec710(short k, O *src)
{
    O *o;
    short i;
    unsigned char s2;
    short n;
    switch (k) {
    case 0: s2 = 0; n = 1; break;
    case 8: s2 = 1; n = 1; break;
    case 9: s2 = 2; n = 1; break;
    case 5: s2 = 0; n = 0; break;
    case 6: s2 = 3; n = 2; break;
    case 7: s2 = 4; n = 4; break;
    }
    for (i = 0; i < n; i++) {
        o = func_80018448();
        if (o != 0) {
            o->a = 1;
            o->c2 = 0x5c;
            o->c3 = s2;
            o->cc = i;
            o->d10 = src->d10;
            o->d14 = src->d14;
            o->d18 = src->d18;
            o->d90 = src;
        }
    }
}
