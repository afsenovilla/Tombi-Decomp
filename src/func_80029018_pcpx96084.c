// VERSION PCPX_96084 tomba 80029018 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// End-of-demo trigger: once the counter D_800DE0E6 is below 2 (0 becomes 3) the player is put in state 5/0x40
// (subtype 0 only); when D_800DE0E6 reaches 1, D_800DE4A3 = 2 shows the TO BE CONTINUED card (task 0x80021050).
extern short D_800CCCDC;
extern unsigned char D_800DE0E6, D_800DE4A3;
extern unsigned char D_800F4FDC, D_800F4FD9, D_800F4FDD;

void func_80029018_pcpx96084(unsigned char *o)
{
    unsigned char s = o[6];
    switch (s) {
    case 0:
        if (D_800CCCDC == 0) {
            unsigned char *c = &D_800DE0E6;
            if (*c < 2) {
                o[6] = s + 1;
                if (*c == 0) *c = 3;
                if (o[7] == 0) {
                    D_800F4FDC = 5;
                    D_800F4FD9 = 0;
                    D_800F4FDD = 0x40;
                }
            }
        }
        break;
    case 1:
        if (D_800DE0E6 == 1) {
            D_800DE4A3 = 2;
            o[6]++;
        }
        break;
    case 2:
        break;
    case 3:
        break;
    }
}
#endif
