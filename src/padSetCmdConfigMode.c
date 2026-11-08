// FUNC 8006ada8 32 MAIN0
// MATCHING 8006ada8 32
// VERSION SCUS_94236 scus94236 8006A358 32
// VERSION BETA_980203 beta980203 80068A9C 32
#if defined(VERSION_SCUS_94236)
// USA version of PAL main0 8006ADA8 (padSetCmdConfigMode.c): PAL C of src/padSetCmdConfigMode.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
void padSetCmdConfigMode(signed char *o, signed char v)
{
    o[0x36] = 0x43;
    *(signed char **)(o + 0x2c) = o + 0x24;
    o[0x24] = v;
    o[0x35] = 1;
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 8006ADA8 (padSetCmdConfigMode.c): SCUS_94236 branch of src/padSetCmdConfigMode.c [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL main0 8006ADA8 (padSetCmdConfigMode.c): PAL C of src/padSetCmdConfigMode.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
void padSetCmdConfigMode(signed char *o, signed char v)
{
    o[0x36] = 0x43;
    *(signed char **)(o + 0x2c) = o + 0x24;
    o[0x24] = v;
    o[0x35] = 1;
}
#else
void padSetCmdConfigMode(signed char *o, signed char v)
{
    o[0x37] = 0x43;
    *(signed char **)(o + 0x2c) = o + 0x24;
    o[0x24] = v;
    o[0x36] = 1;
}
#endif
