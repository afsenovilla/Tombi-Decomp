// FUNC 80069050 204 MAIN0
// MATCHING 80069050 204
typedef struct C { char p0[0x10]; struct C *self; char p1[0x1c]; char *q; char p2[3]; unsigned char a, b, c; char p3[15]; unsigned char st; } C;
extern C *(*DAT_800981c0)(void);
int FUN_80069050(void)
{
    C *c = DAT_800981c0();
    if (c->a || c->b || (c != c->self && c->c) || *c->q) {
        switch (c->st) {
        case 3: return 1;
        case 2: return 1;
        case 6: return 4;
        }
    }
    return c->st;
}
