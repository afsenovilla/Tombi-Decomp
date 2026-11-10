// VERSION PCPX_96084 tomba 8009DD48 192
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
// SPU read-back of slot i (0..16) whose state is 2: transfer start address / size from the slot tables, state 1.
extern unsigned char D_800E2568[];
extern unsigned int D_800E619C[];
extern unsigned int D_800E6158[];
extern void func_800A0390_pcpx96084(int);
extern unsigned int SpuSetTransferStartAddr(unsigned int);
extern unsigned int SpuRead(unsigned char *, unsigned int);
extern void _spu_setInTransfer(int);

int func_8009DD48_pcpx96084(unsigned char *buf, unsigned short n)
{
    short i;
    if (n <= 16) {
        i = n;
        if (D_800E2568[i] == 2) {
            unsigned int a = D_800E619C[i];
            func_800A0390_pcpx96084(0);
            SpuSetTransferStartAddr(a);
            SpuRead(buf, D_800E6158[i]);
            D_800E2568[i] = 1;
            return i;
        }
    }
    _spu_setInTransfer(0);
    return -1;
}
#endif
