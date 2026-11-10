// VERSION PCPX_96084 tomba 800206A8 292
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Gameplay frame: selects the packet buffer of the current frame (0x18000 bytes each), then, unless frozen, the
// frame counter and the object updates; the HUD and camera unless paused, and the scene draw unless in mode 2.
extern short D_1F8002A8;
extern unsigned short D_1F8002AC;
extern short D_1F800126;
extern int D_80100DC0;
extern char D_80100DC4[];
extern void func_8002142C(void);
extern void func_80041C74_pcpx96084(void), func_80021660_pcpx96084(void), func_80024734_pcpx96084(void);
extern void func_8002FDE4_pcpx96084(void), func_80036F10_pcpx96084(void), func_800597FC_pcpx96084(void);
extern void updateObjectsLayer7(void), updateInventoryOverlay(void), updateObjectsUnlayered(void);
extern void func_8006EA2C_pcpx96084(void), func_80025F14_pcpx96084(void), func_80024B0C_pcpx96084(void);
extern void func_8005A55C(void);

void func_800206A8_pcpx96084(void)
{
    D_80100DC0 = (int)(D_80100DC4 + D_1F8002A8 * 0x18000) & 0xffffff;
    func_8002142C();
    if (D_1F800126 == 0) {
        D_1F8002AC++;
        func_80041C74_pcpx96084();
        func_80021660_pcpx96084();
        func_80024734_pcpx96084();
        func_8002FDE4_pcpx96084();
        func_80036F10_pcpx96084();
        func_800597FC_pcpx96084();
        updateObjectsLayer7();
        updateInventoryOverlay();
        updateObjectsUnlayered();
        func_8006EA2C_pcpx96084();
    }
    if (D_1F800126 != 1) func_80025F14_pcpx96084();
    if (D_1F800126 == 0) func_80024B0C_pcpx96084();
    if (D_1F800126 != 2) func_8005A55C();
}
#endif
