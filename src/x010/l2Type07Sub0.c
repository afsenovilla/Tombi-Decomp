// FUNC 80121eb8 116 X010
// MATCHING 80121eb8 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void l2Type07Sub0Step(O *);
extern void l2Type07Sub0End(O *);
void l2Type07Sub0(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        l2Type07Sub0Step(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { l2Type07Sub0End(p); p->s = o->s; p = p->next; }
    }
}
