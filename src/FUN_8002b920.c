// FUNC 8002b920 144 MAIN0
// MATCHING 8002b920 144
extern char *ObjAlloc(void);

void FUN_8002b920(char *o)
{
    char *n = ObjAlloc();
    unsigned char t;
    if (n != 0) {
        n[0] = 1;
        n[2] = 0xd;
        n[3] = 1;
        n[0xc] = o[2];
        t = o[0xf];
        *(char **)(n + 0x90) = o;
        n[0xf] = t - 1;
        *(short *)(n + 0x12) = *(short *)(o + 0x12);
        *(short *)(n + 0x16) = *(short *)(o + 0x16);
        *(short *)(n + 0x1a) = *(short *)(o + 0x1a);
        *(short *)(n + 0xac) = *(short *)(o + 0xac);
    }
}
