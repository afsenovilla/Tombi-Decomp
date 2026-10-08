// FUNC 8012423c 116 X004
// MATCHING 8012423c 116
extern void FUN_801242b0(char *);
extern void FUN_801247d8(char *);
extern void FUN_80124954(char *);

void FUN_8012423c(char *o)
{
    char *p;
    if (o[3] == 0) {
        FUN_801242b0(o);
        p = *(char **)(o + 0x94);
        FUN_801247d8(p);
        p[6] = o[6];
        p = *(char **)(p + 0x94);
        FUN_80124954(p);
        p[6] = o[6];
    }
}
