// FUNC 800ef490 84 X000
// MATCHING 800ef490 84
// VERSION SLPS_01144 x00 800EF5FC 84
// VERSION SLPM_80165 x00 800ED32C 84
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 800EF490 (setFacingFromPad.c): 128 -> 32768, 32 -> 8192.
extern volatile unsigned short DAT_8009d670;
void setFacingFromPad(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &DAT_8009d670;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x2000) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x8000) ? 1 : (v | 2);
    }
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800EF490 (setFacingFromPad.c): pad bits 0x2000/0x8000 (PAL 0x20/0x80).
extern volatile unsigned short DAT_8009d670;
void setFacingFromPad(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &DAT_8009d670;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x2000) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x8000) ? 1 : (v | 2);
    }
}
#else
extern volatile unsigned short DAT_8009d670;
void setFacingFromPad(char *o)
{
    unsigned short v = *(unsigned short *)(o + 0x2e) & 1;
    volatile unsigned short *p = &DAT_8009d670;
    *(unsigned short *)(o + 0x2e) = v;
    if (*p & 0x20) {
        *(short *)(o + 0x2e) = 0;
    } else {
        *(unsigned short *)(o + 0x2e) = (*p & 0x80) ? 1 : (v | 2);
    }
}
#endif
