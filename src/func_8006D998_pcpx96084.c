// VERSION PCPX_96084 tomba 8006D998 148
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// On contact (func_80069700) an object not yet in state 2 turns away from the other one: +0x2E is 1 when the other
// object's X is the larger, then the state bytes restart.
extern short func_80069700(unsigned char *o, unsigned char *e);

void func_8006D998_pcpx96084(unsigned char *o, unsigned char *e)
{
    if (func_80069700(o, e) != 0 && !(o[0] & 2)) {
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
