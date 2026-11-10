// VERSION SLPM_80165 slpm80165 80017F54 164
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8001DCA4_pcpx96084 (src/func_8001DCA4_pcpx96084.c): debug text window setup (buffer
// D_800893D8, CLUT 0x170, 0x1F0; PrePre 8 0x3F0, 0x1F8), height 0x25 or 0x15 by func_8005453C_slpm80165.
// the window struct at D_8009A278 (buffer +0, lines +8); splat names the other fields one by one
typedef struct {
    char *buf;
    short w, h;
    short lines;
} Win80017F54;
extern Win80017F54 D_8009A278;
extern short D_8009A27C, D_8009A27E, D_8009A288, D_8009A28A, D_8009A28C, D_8009A28E;
extern char D_800893D8[];
extern int func_8005453C_slpm80165(void);

void func_80017F54(void)
{
    D_8009A278.buf = D_800893D8;
    D_8009A28C = 0x40;
    D_8009A28E = 0x1E;
    D_8009A27C = 0x170;
    D_8009A288 = 0;
    D_8009A28A = 0;
    D_8009A27E = 0x1F0;
    D_8009A278.lines = (func_8005453C_slpm80165() == 1 || func_8005453C_slpm80165() == 2) ? 0x25 : 0x15;
}
#endif
