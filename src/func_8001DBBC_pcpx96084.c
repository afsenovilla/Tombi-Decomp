// VERSION PCPX_96084 tomba 8001DBBC 232
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Text window init (D_800E5154): glyph source D_800CCCE0, size 0x40 x 0x1E, CLUT at (0x3F0, 0x1F8), and the two
// draw-mode packets with texture page 0x25 or 0x15 (by func_8008A79C_pcpx96084).
typedef struct {
    short w0; short pad2; void *src; char p8[0x20 - 8]; short x20, y20; char p24[0x2a - 0x24]; short w2a, w2c, w2e, w30;
} TB;
extern TB D_800E5154;
extern char D_800E515C[];
extern char D_800CCCE0[];
extern int func_8008A79C_pcpx96084(void);
extern void SetDrawMode(void *, int, int, int, void *);

void func_8001DBBC_pcpx96084(void)
{
    unsigned short tp;
    D_800E5154.src = D_800CCCE0;
    D_800E5154.w2e = 0x40;
    D_800E5154.w30 = 0x1e;
    D_800E5154.x20 = 0x3f0;
    D_800E5154.w0 = 0;
    D_800E5154.w2a = 0;
    D_800E5154.w2c = 0;
    D_800E5154.y20 = 0x1f8;
    if (func_8008A79C_pcpx96084() == 1 || func_8008A79C_pcpx96084() == 2) tp = 0x25;
    else tp = 0x15;
    SetDrawMode(D_800E515C, 0, 0, tp, 0);
    SetDrawMode(D_800E515C + 12, 0, 0, tp, 0);
}
#endif
