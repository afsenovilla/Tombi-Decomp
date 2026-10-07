// FUNC 8012f1b4 116 X000
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void FUN_8012f228(O *);
extern void FUN_8012f36c(O *);
void FUN_8012f1b4(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        FUN_8012f228(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { FUN_8012f36c(p); p->s = o->s; p = p->next; }
    }
}
