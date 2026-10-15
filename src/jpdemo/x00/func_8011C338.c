// JPDEMO x00 8011C338 168
// Demo version of PAL x000 80126198 (playerContactCat2Type15.c): attached object at +0xd0 (PAL +0xe4).
extern short pushObjectOutOfObjectBoxA(void);
extern void spawnImpactEffect(int, int, int, int);

void playerContactCat2Type15(char *o, char *p)
{
    short r = pushObjectOutOfObjectBoxA();
    if (r != 0) {
        if (o[0xa6] & 2)
            o[0xa6] = o[0xa6] + 2;
        if (r == 1 && o[0xac] == r) {
            p[0] = 2;
            p[4] = 2;
            p[5] = 0;
            p[6] = 0;
            p[0x69] = 0;
            *(char **)(o + 0xd0) = p;
            o[0xac] = 2;
            spawnImpactEffect(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
        }
    }
}
