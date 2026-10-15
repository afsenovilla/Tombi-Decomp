// FUNC 80122084 116 X010
// MATCHING 80122084 116
extern void area10Type07NearSfx(char *);
extern void FUN_8012252c(char *);
extern void area10Type07StepBPart(char *);

void FUN_80122084(char *o)
{
    char *p;
    if (o[3] == 0) {
        area10Type07NearSfx(o);
        p = *(char **)(o + 0x94);
        FUN_8012252c(p);
        p[6] = o[6];
        p = *(char **)(p + 0x94);
        area10Type07StepBPart(p);
        p[6] = o[6];
    }
}
