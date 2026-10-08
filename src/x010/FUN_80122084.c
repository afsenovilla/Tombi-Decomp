// FUNC 80122084 116 X010
// MATCHING 80122084 116
extern void FUN_801220f8(char *);
extern void FUN_8012252c(char *);
extern void FUN_801226a8(char *);

void FUN_80122084(char *o)
{
    char *p;
    if (o[3] == 0) {
        FUN_801220f8(o);
        p = *(char **)(o + 0x94);
        FUN_8012252c(p);
        p[6] = o[6];
        p = *(char **)(p + 0x94);
        FUN_801226a8(p);
        p[6] = o[6];
    }
}
