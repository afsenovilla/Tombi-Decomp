// FUNC 801260f8 160 X000
extern short FUN_80043464(char *, unsigned char *);
void FUN_801260f8(char *o, unsigned char *p)
{
    short s;
    if (p[2] == 0x12 && p[4] == 0)
        return;
    s = FUN_80043464(o, p);
    if (s == -1)
        return;
    if (s == 3 && o[0xac] == 1) {
        p[0] = 2;
        p[4] = 2;
        p[5] = 0;
        p[6] = 0;
        p[0x69] = 0;
        *(unsigned char **)(o + 0xe4) = p;
        o[0xac] = 2;
    }
}
