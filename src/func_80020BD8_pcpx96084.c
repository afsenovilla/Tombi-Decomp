// VERSION PCPX_96084 tomba 80020BD8 152
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Gameplay frame: frame counter, the current 0x18000-byte primitive buffer, then the object layers.
typedef struct { char b[0x18000]; } PrimBuf80020BD8;
extern unsigned short D_1F8002AC;
extern short D_1F8002A8;
extern PrimBuf80020BD8 D_80100DC4[];
extern int D_80100DC0;
extern void func_80041C74_pcpx96084(void);
extern void func_80025F14_pcpx96084(void);
extern void updateObjectsLayer7(void);
extern void func_8002FDE4_pcpx96084(void);
extern void func_80021660_pcpx96084(void);
extern void updateObjectsUnlayered(void);
extern void func_8005A55C(void);

void func_80020BD8_pcpx96084(void)
{
    D_1F8002AC++;
    D_80100DC0 = (int)&D_80100DC4[D_1F8002A8] & 0xFFFFFF;
    func_80041C74_pcpx96084();
    func_80025F14_pcpx96084();
    updateObjectsLayer7();
    func_8002FDE4_pcpx96084();
    func_80021660_pcpx96084();
    updateObjectsUnlayered();
    func_8005A55C();
}
#endif
