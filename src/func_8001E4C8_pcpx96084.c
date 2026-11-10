// VERSION PCPX_96084 tomba 8001E4C8 148
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned char D_800E2B28;
extern int D_800E43D4;
extern void func_8001E55C(void *, unsigned short *);
extern void func_8001E6A8(void *, unsigned short *);
extern void func_8001E7F0(void *, unsigned short *);

void func_8001E4C8_pcpx96084(void *a, unsigned short *b)
{
    if (D_800E43D4 & D_800E2B28) return;
    switch (*b) {
    case 0:
        func_8001E55C(a, b);
        break;
    case 1:
        func_8001E6A8(a, b);
        break;
    case 2:
        func_8001E7F0(a, b);
        break;
    }
}
#endif
