// VERSION SLPM_80165 x00 800E8A68 60
#if defined(VERSION_SLPM_80165)
// Demo-only wrapper: func_800E8AA4(a, x, y, z, 1).
extern void func_800E8AA4(int, short, short, short, int);
void func_800E8A68_slpm80165(int a, short x, short y, short z)
{
    func_800E8AA4(a, x, y, z, 1);
}
#endif
