// VERSION PCPX_96084 tomba 800216F4 180
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Steps the state byte of slot i (D_800DE21C), runs func_800217F0 on it and returns the new state. Slot 2 leaves
// state 0 only in area 0 section 0 (the word D_800DE0D8 = area | section << 16).
extern unsigned char D_800DE21C[];
extern int D_800DE0D8;
extern void func_800217F0(int i);

unsigned char func_800216F4_pcpx96084(int i)
{
    switch (D_800DE21C[i]) {
    case 0:
        if (i == 2) {
            if (D_800DE0D8 == 0) D_800DE21C[i] = 3;
        } else {
            D_800DE21C[i] = 3;
        }
        break;
    case 1:
    case 2:
        break;
    case 3:
        D_800DE21C[i] = 1;
        break;
    case 4:
        D_800DE21C[i] = 2;
        break;
    }
    func_800217F0(i);
    return D_800DE21C[i];
}
#endif
