// FUNC 8006ada8 32 MAIN0
// MATCHING 8006ada8 32
void FUN_8006ada8(signed char *o, signed char v)
{
    o[0x37] = 0x43;
    *(signed char **)(o + 0x2c) = o + 0x24;
    o[0x24] = v;
    o[0x36] = 1;
}
