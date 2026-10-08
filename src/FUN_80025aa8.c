// FUNC 80025aa8 240 MAIN0
// MATCHING 80025aa8 240
extern void mcpy(void *, void *, int);
void FUN_80025aa8(int *src, char *dst)
{
    unsigned short sz[8];
    short i = 0;
    int v;
    unsigned short *p;
    sz[0] = 0x20; sz[1] = 0x28; sz[2] = 0x28; sz[3] = 0x34;
    sz[4] = 0x28; sz[5] = 0x30; sz[6] = 0x40; sz[7] = 0x54;
    do {
        v = *src++;
        if (v != 0) {
            p = (unsigned short *)(((i << 16) >> 15) + (int)sz);
            *p = *p * v;
            mcpy(dst, src, *p);
            dst += *p & 0xfffc;
            src = (int *)((char *)src + (*p & 0xfffc));
        }
        i++;
    } while (i < 8);
}

