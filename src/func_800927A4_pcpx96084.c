// VERSION PCPX_96084 tomba 800927A4 128
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
// Psy-Q libc2 strncmp (the PAL exe does not link it); the shape of src/strcmp.c.
int func_800927A4_pcpx96084(const char *a, const char *b, int n)
{
    if ((a == 0) || (b == 0)) {
        if (a == b) {
            return 0;
        }
        return (a == 0) ? -1 : 1;
    }
    while (--n >= 0 && *a == *b++) {
        if (*a++ == '\0') {
            return 0;
        }
    }
    return (n < 0) ? 0 : *a - *--b;
}
#endif
