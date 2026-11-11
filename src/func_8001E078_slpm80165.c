// VERSION SLPM_80165 slpm80165 8001E078 304
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: opens and plays the BGM of the current area / section (D_8006B9C4 tables, 4-byte BGM entries at
// D_8006B8C4: sequence, then the index of its volume in the scratchpad table 0x1F8003A8); bits 0xF00 of the
// argument: 0 plays at volume 0x40, 0x100 starts silent (D_80088FEC set, for a fade-in).
extern unsigned short D_8009A810, D_8009A812;
extern short *D_8006B9C4[];
extern unsigned char D_8006B8C4[], D_8006B8C5[];
extern unsigned short D_1F8003A8[];
extern unsigned short D_8009A7A0;
extern int D_8009B580[];
extern short D_800A2A4C;
extern short D_80088FEC;
extern short SsSeqOpen(int, short);
extern void SsSetMVol(int, int);
extern void SsSeqSetVol(short, int, int);
extern void SsSeqPlay(short, int, int);

int func_8001E078(int mode)
{
    int t;
    register short *p asm("$2") = D_8006B9C4[D_8009A810];
    p += D_8009A812;
    t = *p;
    D_8009A7A0 = D_1F8003A8[D_8006B8C5[t * 4]];
    D_800A2A4C = SsSeqOpen(D_8009B580[D_8006B8C4[t * 4]], (short)D_8009A7A0);
    SsSetMVol(100, 100);
    switch (mode & 0xf00) {
    case 0:
        SsSeqSetVol(D_800A2A4C, 0x40, 0x40);
        D_80088FEC = 0;
        break;
    case 0x100:
        SsSeqSetVol(D_800A2A4C, 0, 0);
        D_80088FEC = 1;
        break;
    }
    SsSeqPlay(D_800A2A4C, 1, 1);
    return 0;
}
#endif
