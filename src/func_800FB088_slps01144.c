// VERSION SLPS_01144 x00 800FB088 12
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
void func_800FB088(unsigned char *p)
{
    *(int *)(p + 0x8C) = p[0x88];
}
#endif
