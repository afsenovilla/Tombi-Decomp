// FUNC 80124ad8 116 X004
// MATCHING 80124ad8 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void area4Type07StepC(O *);
extern void copyParentB8cToD8c(O *);
void area4Type07StepCChain(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        area4Type07StepC(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { copyParentB8cToD8c(p); p->s = o->s; p = p->next; }
    }
}
