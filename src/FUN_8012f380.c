// FUNC 8012f380 116 X000
// MATCHING 8012f380 116
extern void FUN_8012f3f4(char *);
extern void FUN_8012f864(char *);
extern void FUN_8012f9e0(char *);

void FUN_8012f380(char *o)
{
    char *p;
    if (o[3] == 0) {
        FUN_8012f3f4(o);
        p = *(char **)(o + 0x94);
        FUN_8012f864(p);
        p[6] = o[6];
        p = *(char **)(p + 0x94);
        FUN_8012f9e0(p);
        p[6] = o[6];
    }
}
