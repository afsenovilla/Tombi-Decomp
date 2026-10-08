// FUNC 80122804 116 X010
// MATCHING 80122804 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void FUN_80122878(O *);
extern void FUN_80122b2c(O *);
void FUN_80122804(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        FUN_80122878(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { FUN_80122b2c(p); p->s = o->s; p = p->next; }
    }
}
