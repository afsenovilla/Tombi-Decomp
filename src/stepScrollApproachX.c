// FUNC 800284d8 112 MAIN0
// MATCHING 800284d8 112
// VERSION PCPX_96084 tomba 80026D24 112
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 800284D8 (stepScrollApproachX.c): PAL C of src/stepScrollApproachX.c (0.929 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct S { char pad[0x20]; int v; char pad2[0x6d - 0x24]; signed char m; char pad3; signed char t; } S;

int stepScrollApproachX(S *s)
{
    switch (s->m) {
    case 0:
        if ((s->t << 8) < s->v) {
            s->v = s->v - 0x80;
            return 0;
        }
        return 1;
    case 1:
        if (s->v >= (s->t << 8)) {
            return 1;
        }
        s->v = s->v + 0x80;
        return 0;
    }
    return 0;
}
#else
typedef struct S { char pad[0x20]; int v; char pad2[0x6d - 0x24]; signed char m; char pad3; signed char t; } S;

int stepScrollApproachX(S *s)
{
    switch (s->m) {
    case 0:
        if ((s->t << 8) < s->v) {
            s->v = s->v - 0x100;
            return 0;
        }
        return 1;
    case 1:
        if (s->v >= (s->t << 8)) {
            return 1;
        }
        s->v = s->v + 0x100;
        return 0;
    }
    return 0;
}
#endif
