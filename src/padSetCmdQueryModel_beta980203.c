// VERSION BETA_980203 beta980203 80068ABC 20
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/padSetCmdQueryModel_scus94236.c (padSetCmdQueryModel, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL padSetCmdQueryModel.c (src/padSetCmdQueryModel.c): o[0x37]=>o[0x36]; o[0x36] = 0;=>o[0x35] = 0;.
void padSetCmdQueryModel(char *o)
{
    o[0x36] = 0x45;
    *(int *)(o + 0x2c) = 0;
    o[0x35] = 0;
}
#endif
