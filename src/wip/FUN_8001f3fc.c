// FUNC 8001f3fc 192 MAIN0
typedef struct E { unsigned short a; unsigned short b; } E;
extern E DAT_800a3d00[];
extern short DAT_8009d688;
extern unsigned short DAT_800a3f90;
extern short DAT_8009f2d0;
int FUN_8001f3fc(unsigned short a, unsigned short b)
{
    unsigned int u = DAT_800a3f90;
    int i = 0;
    unsigned short cnt;
    if (0 < DAT_8009d688) {
        do {
            if (DAT_800a3d00[(short)u].a == a)
                return -1;
            u = (u + 1) & 0x7f;
            i++;
        } while (i < DAT_8009d688);
    }
    cnt = DAT_8009d688 + 1;
    DAT_800a3d00[DAT_8009f2d0].a = a;
    DAT_800a3d00[DAT_8009f2d0].b = b;
    DAT_8009f2d0 = (DAT_8009f2d0 + 1) & 0x7f;
    DAT_8009d688 = cnt;
    return 0;
}
