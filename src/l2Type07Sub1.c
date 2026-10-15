// FUNC 8012f380 116 X000
// MATCHING 8012f380 116
extern void l2Type07Sub1StepA(char *);
extern void l2Type07Sub1StepB(char *);
extern void l2Type07Sub1StepC(char *);

void l2Type07Sub1(char *o)
{
    char *p;
    if (o[3] == 0) {
        l2Type07Sub1StepA(o);
        p = *(char **)(o + 0x94);
        l2Type07Sub1StepB(p);
        p[6] = o[6];
        p = *(char **)(p + 0x94);
        l2Type07Sub1StepC(p);
        p[6] = o[6];
    }
}
