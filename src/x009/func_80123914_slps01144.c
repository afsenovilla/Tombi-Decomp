// near-twin of PAL playerStepEntrySfxAnim4State5 (tools/jp_autofix.py)
// VERSION SLPS_01144 x09 80123914 104
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct O { unsigned char b0, b1; char pad[4]; unsigned char state; } O;
extern unsigned char D_8009E50C, D_8009E4E2;
extern void func_80020E38();

void func_80123914(O *o)
{
    if (o->state != 0)
        return;
    func_80020E38();
    if (D_8009E50C == 33 && D_8009E4E2 == 1)
        o->b1 = 0;
}
#endif
