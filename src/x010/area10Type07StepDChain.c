// FUNC 80122b40 116 X010
// MATCHING 80122b40 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void area10Type07StepDPart(O *);
extern void area10Type07StepDPart(O *);
void area10Type07StepDChain(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        area10Type07StepDPart(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { area10Type07StepDPart(p); p->s = o->s; p = p->next; }
    }
}
