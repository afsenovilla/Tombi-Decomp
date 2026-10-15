// FUNC 801260f8 160 X000
// MATCHING 801260f8 160
// VERSION SLPS_01144 x00 80124E40 160
// VERSION SLPM_80165 x00 8011C298 160
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 801260F8 (playerContactCat2Type12.c): 228 -> 212.
extern short objectBoxOverlapSide(char *, unsigned char *);
void playerContactCat2Type12(char *o, unsigned char *p)
{
    short s;
    if (p[2] == 0x12 && p[4] == 0)
        return;
    s = objectBoxOverlapSide(o, p);
    if (s == -1)
        return;
    if (s == 3 && o[0xac] == 1) {
        p[0] = 2;
        p[4] = 2;
        p[5] = 0;
        p[6] = 0;
        p[0x69] = 0;
        *(unsigned char **)(o + 0xd4) = p;
        o[0xac] = 2;
    }
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL x000 801260F8 (playerContactCat2Type12.c): attached object at +0xd0 (PAL +0xe4).
extern short objectBoxOverlapSide(char *, unsigned char *);
void playerContactCat2Type12(char *o, unsigned char *p)
{
    short s;
    if (p[2] == 0x12 && p[4] == 0)
        return;
    s = objectBoxOverlapSide(o, p);
    if (s == -1)
        return;
    if (s == 3 && o[0xac] == 1) {
        p[0] = 2;
        p[4] = 2;
        p[5] = 0;
        p[6] = 0;
        p[0x69] = 0;
        *(unsigned char **)(o + 0xd0) = p;
        o[0xac] = 2;
    }
}
#else
extern short objectBoxOverlapSide(char *, unsigned char *);
void playerContactCat2Type12(char *o, unsigned char *p)
{
    short s;
    if (p[2] == 0x12 && p[4] == 0)
        return;
    s = objectBoxOverlapSide(o, p);
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
#endif
