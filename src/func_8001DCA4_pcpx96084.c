// VERSION PCPX_96084 tomba 8001DCA4 164
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Debug text window setup: buffer, size, position, and a height of 0x25 or 0x15 by func_8008A79C (1 or 2: 0x25).
typedef struct {
    char *buf;
    short w, h;
    short lines;
    short pad[3];
    short x, y;
    short cw, ch;
} Win8001DCA4;
extern Win8001DCA4 D_800DDB7C;
extern char D_800CCCE0[];
extern int func_8008A79C_pcpx96084(void);

void func_8001DCA4_pcpx96084(void)
{
    D_800DDB7C.buf = D_800CCCE0;
    D_800DDB7C.cw = 0x40;
    D_800DDB7C.ch = 0x1E;
    D_800DDB7C.w = 0x3F0;
    D_800DDB7C.x = 0;
    D_800DDB7C.y = 0;
    D_800DDB7C.h = 0x1F8;
    D_800DDB7C.lines = (func_8008A79C_pcpx96084() == 1 || func_8008A79C_pcpx96084() == 2) ? 0x25 : 0x15;
}
#endif
