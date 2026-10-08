// FUNC 800eee90 124 X011
// MATCHING 800eee90 124
typedef struct { char p0[4]; char b4, b5, b6; } R;
typedef struct { char p0[0x9d]; char b9d; char p1[0xac - 0x9e]; unsigned char bac; char p2[0xc6 - 0xad]; char bc6, bc7; char p3[0xe3 - 0xc8]; char be3; } TO;
extern R *Y;
extern R *X;
extern int G934;
extern char *P330;

void FUN_800eee90(TO *o)
{
    if (o->bac > 1) {
        X = Y;
        X->b4 = 2;
        X->b5 = 2;
        X->b6 = 0;
    }
    o->bac = 0;
    G934 = 0;
    o->bc7 = 1;
    o->b9d = 0;
    o->bc6 = 0;
    o->be3 = 0;
    *P330 = 0;
}
