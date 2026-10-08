// FUNC 8006bf4c 44 MAIN0
// MATCHING 8006bf4c 44
int func_8006BF4C(unsigned char *o)
{
    int r;
    if (*(unsigned short *)(o + 0xe6) == 0 || o[0x46] != 0xff)
        r = 1;
    else
        r = 0;
    return r;
}
