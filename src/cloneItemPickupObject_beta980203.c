// VERSION BETA_980203 beta980203 8002DCA0 136
#if defined(VERSION_BETA_980203)
// Beta version: PAL cloneItemPickupObject.c without the n[0xc] copy and with field 0x16 copied minus 38.
extern char *ObjAlloc(void);

void cloneItemPickupObject(char *o)
{
    char *n = ObjAlloc();
    unsigned char t;
    if (n != 0) {
        n[0] = 1;
        n[2] = 0xd;
        n[3] = 1;
        t = o[0xf];
        *(char **)(n + 0x90) = o;
        n[0xf] = t - 1;
        *(short *)(n + 0x12) = *(short *)(o + 0x12);
        *(short *)(n + 0x16) = *(short *)(o + 0x16) - 38;
        *(short *)(n + 0x1a) = *(short *)(o + 0x1a);
        *(short *)(n + 0xac) = *(short *)(o + 0xac);
    }
}
#endif
