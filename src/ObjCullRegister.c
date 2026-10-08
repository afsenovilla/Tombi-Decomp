// FUNC 800202b4 296 MAIN0
// MATCHING 800202b4 296
extern unsigned short DAT_1f800176, DAT_1f800186;
extern void P1(unsigned char *), P2(unsigned char *), P3(unsigned char *), P4(unsigned char *), P5(unsigned char *), P7(unsigned char *), P8(unsigned char *);

int ObjCullRegister(unsigned char *o)
{
    if (o[0] == 0)
        return 0;
    o[1] = 0;
    if ((unsigned short)(*(unsigned short *)(*(int *)(o + 0x40) + 2) - DAT_1f800176 + 0x40) >= 0x1c1)
        return 0;
    if ((unsigned short)(DAT_1f800186 - *(unsigned short *)(o + 0x16) + 0x40) >= 0x171)
        return 0;
    o[1] = 1;
    switch (o[0x1c] & 0x7f) {
    case 1: P1(o); return 1;
    case 2: P2(o); return 1;
    case 3: P3(o); return 1;
    case 4: P4(o); return 1;
    case 5: P5(o); return 1;
    case 7: P7(o); return 1;
    case 8: P8(o);
    default: return 1;
    }
}
