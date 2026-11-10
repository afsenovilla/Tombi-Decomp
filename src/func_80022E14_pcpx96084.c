// VERSION PCPX_96084 tomba 80022E14 288
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Sound init: libsnd tables, the sound bank header (0x801F6000) and body (0x80140000) from the CD, the VAB opened
// and transferred, master / CD volumes, the area BGM, and the eight VAB slots and the sequence id cleared.
typedef struct { short l, r; } VOL;
typedef struct { unsigned int mask; VOL mvol, mvolmode, mvolx; VOL cdvol; int cdrev, cdmix; } SPUATTR;
extern SPUATTR D_800C8238;
extern char D_800E43DC[];
extern char D_800AFCD4[];
extern short D_800E43C8;
extern short D_800E2B34[];
extern short D_800E518C;
extern void func_80095538_pcpx96084(void);
extern void SsSetTableSize(char *, int, int);
extern void SsSetTickMode(int);
extern void func_8009D848_pcpx96084(int);
extern int func_80023E88_pcpx96084(void *, void *);
extern short SsVabOpenHeadSticky(void *, int, int);
extern int func_8009DD48_pcpx96084(void *, int);
extern void func_8009DE08_pcpx96084(int);
extern void SpuSetCommonAttr(SPUATTR *);
extern void startAreaBgm(void);

void func_80022E14_pcpx96084(void)
{
    int i;
    short *p;
    func_80095538_pcpx96084();
    SsSetTableSize(D_800E43DC, 4, 5);
    SsSetTickMode(1);
    func_8009D848_pcpx96084(0);
    {
        char *h = D_800AFCD4;
        void *b = (void *)0x801F6000;
        asm("" : "=r"(h) : "0"(h));
        func_80023E88_pcpx96084(b, h);
        func_80023E88_pcpx96084((void *)0x80140000, h + 20);
    }
    D_800E43C8 = SsVabOpenHeadSticky((void *)0x801F6000, 15, 0x1000);
    func_8009DD48_pcpx96084((void *)0x80140000, D_800E43C8);
    func_8009DE08_pcpx96084(1);
    D_800C8238.mask = 0x2c3;
    D_800C8238.mvol.l = 0x3fff;
    D_800C8238.mvol.r = 0x3fff;
    D_800C8238.cdvol.l = 0x7fff;
    D_800C8238.cdvol.r = 0x7fff;
    D_800C8238.cdmix = 1;
    SpuSetCommonAttr(&D_800C8238);
    startAreaBgm();
    {
        short m = -1;
        i = 7;
        p = &D_800E2B34[7];
        for (; i >= 0; i--) *p-- = m;
    }
    D_800E518C = -1;
}
#endif
