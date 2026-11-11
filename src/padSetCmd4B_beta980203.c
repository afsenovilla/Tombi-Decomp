// VERSION BETA_980203 beta980203 80068B30 20
#if defined(VERSION_BETA_980203)
// Beta version of src/padSetCmd4B.c (padSetCmd4B, PAL body): pad fields one byte lower.
void padSetCmd4B(char *p)
{
    p[0x36] = 0x4b;
    *(int *)(p + 0x2c) = 0;
    p[0x35] = 0;
}
#endif
