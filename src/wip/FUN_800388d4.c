// FUNC 800388d4 496 MAIN0
/* score 126 (ncheck). b29: game loads a (lw a2,0x1090) before the switch index andi/addiu/sltiu; ours schedules the index chain into the idx load chain, and regs differ (game: index a1, b v1, r a3, f t1). Tried: sibling FUN_800387e0 style (int address arithmetic, worse), op int/char/short (andi lost), volatile a load, type brute force (only a=uchar helps, wrong). */
extern unsigned char *DAT_8009f0f0;
extern int DAT_8009d60c;

void FUN_800388d4(unsigned char op)
{
    int tmp;
    unsigned char *o = DAT_8009f0f0;
    int code = DAT_8009d60c;
    int idx;
    int a;
    int b;
    int k;
    int r;
    unsigned char f;
    unsigned char *p;
    unsigned char *g;
    unsigned short x;
    idx = *(unsigned char *)(*(unsigned short *)(o + 0x8a) + code + 1);
    a = *(int *)(o + idx * 4 + 0x1090);
    switch (op) {
    case 0x1c: case 0x1e: case 0x20: case 0x22:
    case 0x24: case 0x26: case 0x28: case 0x2a:
        b = *(int *)(DAT_8009f0f0 + *(unsigned char *)(*(unsigned short *)(o + 0x8a) + code + 2) * 4 + 0x1090);
        f = 0;
        break;
    case 0x1d: case 0x1f: case 0x21: case 0x23:
    case 0x25: case 0x27: case 0x29: case 0x2b:
        p = (unsigned char *)(*(unsigned short *)(o + 0x8a) + code) + 2;
        for (k = 0; k < 4; k++)
            ((char *)&tmp)[k] = p[k];
        b = tmp;
        f = 1;
        break;
    }
    switch (op) {
    case 0x1c: case 0x1d:
        r = a + b;
        break;
    case 0x1e: case 0x1f:
        r = a - b;
        break;
    case 0x20: case 0x21:
        r = a * b;
        break;
    case 0x22: case 0x23:
        r = 0;
        if (b != 0)
            r = a / b;
        break;
    case 0x24: case 0x25:
        r = 0;
        if (b != 0)
            r = a % b;
        break;
    case 0x26: case 0x27:
        r = a & b;
        break;
    case 0x28: case 0x29:
        r = a | b;
        break;
    case 0x2a: case 0x2b:
        r = a ^ b;
        break;
    }
    *(int *)(o + idx * 4 + 0x1090) = r;
    g = DAT_8009f0f0;
    if (r == 0)
        g[0x89] = 0;
    else
        g[0x89] = (r < 0) ? 1 : 2;
    x = *(unsigned short *)(o + 0x8a);
    *(unsigned short *)(o + 0x8a) = f ? x + 6 : x + 3;
}
