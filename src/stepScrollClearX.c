// FUNC 80028b40 68 MAIN0
// MATCHING 80028b40 68
// VERSION PCPX_96084 tomba 80027050 60
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80028B40 (stepScrollClearX.c): SLPM_80165 C of src/stepScrollClearX.c (stepScrollClearX, twin score 0.844), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct S { char pad[0x20]; int v; char pad2[0x6e - 0x24]; char a; char b; } S;

int stepScrollClearX(S *s)
{
    int v = s->v;
    if (v != 0) {
        if (v > 0) {
            v -= 0x80;
        } else {
            v += 0x80;
        }
        s->v = v;
        return 0;
    } else {
        return 1;
        s->b = 0;
        s->a = 0;
    }
}
#else
typedef struct S { char pad[0x20]; int v; char pad2[0x6e - 0x24]; char a; char b; } S;

int stepScrollClearX(S *s)
{
    int v = s->v;
    if (v != 0) {
        if (v > 0) {
            v -= 0x100;
        } else {
            v += 0x100;
        }
        s->v = v;
        return 0;
    } else {
        s->a = 0;
        s->b = 0;
        return 1;
    }
}
#endif
