// FUNC 80124e14 116 X004
// MATCHING 80124e14 116
typedef struct O { char p0[3]; unsigned char t; char p1[2]; unsigned char s; char p2[0x94 - 7]; struct O *next; } O;
extern void area4Type07StepD(O *);
extern void area4Type07StepD(O *);
void FUN_80124e14(O *o)
{
    int i;
    O *p;
    if (o->t == 0) {
        area4Type07StepD(o);
        i = 1;
        p = o->next;
        for (i = 1; i < 3; i++) { area4Type07StepD(p); p->s = o->s; p = p->next; }
    }
}
