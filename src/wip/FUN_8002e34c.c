// FUNC 8002e34c 200 MAIN0
typedef struct A { short n; short x[3]; } A;
typedef struct B { short a; unsigned short m; short c[3]; } B;
extern A DAT_800a4648[];
extern short DAT_800a4de0[];
extern B DAT_800a5de0[];
void FUN_8002e34c(unsigned int p)
{
    int i;
    int iVar1;
    unsigned int u;
    if (DAT_800a4648[p].n != -1) {
        i = 0;
        if (0 < DAT_800a4648[p].n) {
            do {
                iVar1 = *(short *)((char *)DAT_800a4de0 + (p << 10) + i * 8);
                u = DAT_800a5de0[iVar1].m & ~(1 << (p & 0x1f));
                DAT_800a5de0[iVar1].m = u;
                if (u == 0) DAT_800a5de0[iVar1].a = -1;
                i++;
            } while (i < DAT_800a4648[p].n);
        }
        DAT_800a4648[p].n = -1;
    }
}
