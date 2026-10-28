// FUNC 80122804 116 X010
// MATCHING 80122804 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void area10Type07Swing(O *);
extern void copyParentB8cToD8cX10(O *);
void area10Type07SwingChain(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        area10Type07Swing(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { copyParentB8cToD8cX10(p); p->s = o->s; p = p->next; }
    }
}
