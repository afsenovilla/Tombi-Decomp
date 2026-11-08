// FUNC 8006ab78 28 MAIN0
// MATCHING 8006ab78 28
// VERSION SCUS_94236 scus94236 8006A128 28
// VERSION BETA_980203 beta980203 8006886C 28
#if defined(VERSION_SCUS_94236)
// USA version of PAL main0 8006AB78 (padSetCmdActAlign.c): PAL C of src/padSetCmdActAlign.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct S { char p[0x20]; int a; char q[8]; int b; char r[5]; unsigned char c; unsigned char d; } S;
void padSetCmdActAlign(S *o)
{
    o->d = 0x4d;
    o->c = 6;
    o->b = o->a;
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 8006AB78 (padSetCmdActAlign.c): SCUS_94236 branch of src/padSetCmdActAlign.c [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL main0 8006AB78 (padSetCmdActAlign.c): PAL C of src/padSetCmdActAlign.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct S { char p[0x20]; int a; char q[8]; int b; char r[5]; unsigned char c; unsigned char d; } S;
void padSetCmdActAlign(S *o)
{
    o->d = 0x4d;
    o->c = 6;
    o->b = o->a;
}
#else
typedef struct S { char p[0x20]; int a; char q[8]; int b; char r[6]; unsigned char c; unsigned char d; } S;
void padSetCmdActAlign(S *o)
{
    o->d = 0x4d;
    o->c = 6;
    o->b = o->a;
}
#endif
