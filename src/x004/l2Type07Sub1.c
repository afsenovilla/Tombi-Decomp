// FUNC 8012423c 116 X004
// MATCHING 8012423c 116
extern void area4Type07StepA(char *);
extern void l2Type07Sub1StepB(char *);
extern void area4Type07StepB(char *);

void l2Type07Sub1(char *o)
{
    char *p;
    if (o[3] == 0) {
        area4Type07StepA(o);
        p = *(char **)(o + 0x94);
        l2Type07Sub1StepB(p);
        p[6] = o[6];
        p = *(char **)(p + 0x94);
        area4Type07StepB(p);
        p[6] = o[6];
    }
}
