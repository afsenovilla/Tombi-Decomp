// VERSION PCPX_96084 tomba 80036BB0 236
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x017 spawnBreakPieces (src/x017/spawnBreakPieces.c): no category copy, halfword clear at +0x0A.
typedef struct N {
    unsigned char b0, b1, b2, b3;
    char pad4[6];
    unsigned char ba;
    char padb;
    unsigned char bc;
    char padd[2];
    unsigned char bf;
    int x, y, z;
    char pad1c[2];
    short h1e;
    char pad20[0x1c];
    int w3c;
} N;
extern short D_800A7350[];
extern N *func_8001A464_pcpx96084(void);

void func_80036BB0_pcpx96084(N *o, short x, short y, short z)
{
    short *p;
    int i;
    N *n;

    p = D_800A7350;
    i = 0;
    do {
        n = func_8001A464_pcpx96084();
        if (n != 0) {
            n->b0 = 1;
            n->b2 = 0x1e;
            n->x = (x + *p++) << 16;
            n->y = (y + *p++) << 16;
            n->z = z << 16;
            n->h1e = o->h1e;
            *(short *)&n->ba = 0;
            n->b3 = i;
            n->w3c = o->w3c;
            n->bf = o->bf;
        }
        i++;
    } while (i < 5);
}
#endif
