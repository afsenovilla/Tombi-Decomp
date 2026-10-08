// FUNC 80121eb8 116 X010
// MATCHING 80121eb8 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void FUN_80121f2c(O *);
extern void FUN_80122070(O *);
void FUN_80121eb8(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        FUN_80121f2c(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { FUN_80122070(p); p->s = o->s; p = p->next; }
    }
}
