// VERSION BETA_980203 beta980203 80068AD0 32
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/padSetCmdQueryMode_scus94236.c (padSetCmdQueryMode, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL main0 8006ADA8 (padSetCmdConfigMode.c): PAL C of src/padSetCmdConfigMode.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
void padSetCmdQueryMode(signed char *o, signed char v)
{
    o[0x36] = 0x4c;
    *(signed char **)(o + 0x2c) = o + 0x24;
    o[0x24] = v;
    o[0x35] = 1;
}
#endif
