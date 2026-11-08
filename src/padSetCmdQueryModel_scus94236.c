// VERSION SCUS_94236 scus94236 8006A378 20
#if defined(VERSION_SCUS_94236)
// USA version of PAL padSetCmdQueryModel.c (src/padSetCmdQueryModel.c): o[0x37]=>o[0x36]; o[0x36] = 0;=>o[0x35] = 0;.
void padSetCmdQueryModel(char *o)
{
    o[0x36] = 0x45;
    *(int *)(o + 0x2c) = 0;
    o[0x35] = 0;
}
#endif
