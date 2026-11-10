// VERSION SLPM_80165 slpm80165 80017E6C 232
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8001DBBC_pcpx96084 (src/func_8001DBBC_pcpx96084.c): text window init (D_800A2A18), glyph
// source D_800893D8, size 0x40 x 0x1E, CLUT at (0x170, 0x1F0) (PrePre 8: 0x3F0, 0x1F8), and the two draw-mode
// packets with texture page 0x25 or 0x15 by func_8005453C_slpm80165.
// the window struct at D_800A2A18, one symbol per field as splat names them
extern short D_800A2A18, D_800A2A38, D_800A2A3A, D_800A2A42, D_800A2A44, D_800A2A46, D_800A2A48;
extern void *D_800A2A1C;
extern char D_800A2A20[];
extern char D_800893D8[];
extern int func_8005453C_slpm80165(void);
extern void SetDrawMode(void *, int, int, int, void *);

void func_80017E6C(void)
{
    unsigned short tp;
    D_800A2A1C = D_800893D8;
    D_800A2A46 = 0x40;
    D_800A2A48 = 0x1e;
    D_800A2A38 = 0x170;
    D_800A2A18 = 0;
    D_800A2A42 = 0;
    D_800A2A44 = 0;
    D_800A2A3A = 0x1f0;
    if (func_8005453C_slpm80165() == 1 || func_8005453C_slpm80165() == 2) tp = 0x25;
    else tp = 0x15;
    SetDrawMode(D_800A2A20, 0, 0, tp, 0);
    SetDrawMode(D_800A2A20 + 12, 0, 0, tp, 0);
}
#endif
