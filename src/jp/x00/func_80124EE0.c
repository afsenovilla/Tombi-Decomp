// JP x00 80124EE0 168
// Japanese version of PAL x000 80126198 (FUN_80126198.c): 228 -> 212.
extern short pushObjectOutOfObjectBoxA(void);
extern void spawnImpactEffect(int, int, int, int);

void FUN_80126198(char *o, char *p)
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
            *(char **)(o + 0xd4) = p;
            o[0xac] = 2;
            spawnImpactEffect(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
        }
    }
}
