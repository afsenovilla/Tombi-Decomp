// FUNC 8002e34c 200 MAIN0
// MATCHING 8002e34c 200
typedef struct A { short n; short x[3]; } A;
typedef struct B { short a; unsigned short m; short c[3]; } B;
typedef struct C { short id; short x[3]; } C;
extern A DAT_800a4648[];
extern C DAT_800a4de0[][128];
extern B DAT_800a5de0[];
void FUN_8002e34c(unsigned int p)
{
    char pad[4];
    int i;
    int k;
    unsigned int u;
    if (DAT_800a4648[p].n != -1) {
        for (i = 0; i < DAT_800a4648[p].n; i++) {
            k = DAT_800a4de0[p][i].id;
            u = DAT_800a5de0[k].m & ~(1 << p);
            DAT_800a5de0[k].m = u;
            if (u == 0) DAT_800a5de0[k].a = -1;
        }
        DAT_800a4648[p].n = -1;
    }
}
