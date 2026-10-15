// JPDEMO x00 8011C298 160
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
