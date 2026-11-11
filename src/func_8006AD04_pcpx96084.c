// VERSION PCPX_96084 tomba 8006AD04 160
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// func_8006D998_pcpx96084.c with the contact test func_80069E5C (result 1).
extern short func_80069E5C_pcpx96084(unsigned char *o, unsigned char *e);

void func_8006AD04_pcpx96084(unsigned char *o, unsigned char *e)
{
    short r = func_80069E5C_pcpx96084(o, e);
    if (r != 0 && r == 1 && !(o[0] & 2)) {
        o[0] = 2;
        if (*(short *)(*(char **)(e + 0x40) + 2) > *(short *)(*(char **)(o + 0x40) + 2)) {
            *(short *)(o + 0x2E) = 1;
        } else {
            *(short *)(o + 0x2E) = 0;
        }
        o[4] = 2;
        o[5] = 0;
        o[6] = 0;
    }
}
#endif
