// VERSION SCUS_94236 scus94236 8006A3EC 20
#if defined(VERSION_SCUS_94236)
// USA version of PAL padSetCmd4B.c (src/padSetCmd4B.c): p[0x37]=>p[0x36]; p[0x36] = 0;=>p[0x35] = 0;.
void padSetCmd4B(char *p)
{
    p[0x36] = 0x4b;
    *(int *)(p + 0x2c) = 0;
    p[0x35] = 0;
}
#endif
