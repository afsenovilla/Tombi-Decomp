// VERSION SCUS_94236 scus94236 8006A3AC 32
#if defined(VERSION_SCUS_94236)
// USA version of PAL main0 8006ADA8 (padSetCmdConfigMode.c): PAL C of src/padSetCmdConfigMode.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
void padSetCmdQueryAct(signed char *o, signed char v)
{
    o[0x36] = 0x46;
    *(signed char **)(o + 0x2c) = o + 0x24;
    o[0x24] = v;
    o[0x35] = 1;
}
#endif
