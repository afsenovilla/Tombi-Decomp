// VERSION SLPM_80165 slpm80165 80026854 236
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80025A94_pcpx96084 (src/func_80025A94_pcpx96084.c): inventory add, n more of item (counts
// at D_8009AF54, list at D_8009B054, length D_8009B154). A third argument first calls func_8002DC34(item, 0); both
// paths end with func_8001E3D8(10) (written twice: cross-jumping merges the calls).
extern unsigned char D_8009AF54[], D_8009B054[];
extern unsigned short D_8009B154;
extern void func_8002DC34(int, int);
extern void func_8001E3D8(int);

unsigned char func_80026854(int item, int n, int flag)
{
    int i;
    if (flag)
        func_8002DC34(item, 0);
    for (i = 0; i < D_8009B154; i++) {
        if (D_8009B054[i] == item) {
            D_8009AF54[item] += n;
            func_8001E3D8(10);
            return D_8009AF54[item];
        }
    }
    D_8009B054[i] = item;
    D_8009AF54[item] = n;
    {
        unsigned short *q = &D_8009B154;
        (*q)++;
    }
    func_8001E3D8(10);
    return D_8009AF54[item];
}
#endif
