// FUNC 80124070 116 X004
// MATCHING 80124070 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void FUN_801240e4(O *);
extern void FUN_80124228(O *);
void FUN_80124070(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        FUN_801240e4(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { FUN_80124228(p); p->s = o->s; p = p->next; }
    }
}
