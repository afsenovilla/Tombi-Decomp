// FUNC 80069610 400 MAIN0
// MATCHING 80069610 400
// FLAGS -O2 -G0
// AS gnu
/* vsync handler of the pad driver (US func_80069058, libpad/padmain.c): polls both ports. Library code: assembled with GNU as, see padlocal.h. */
typedef struct { unsigned char pad[0xf0]; } E;
typedef struct { unsigned char pad[0xa]; unsigned short a; unsigned short c; unsigned short e; } H;
extern H *PAD_SIO;
extern int D_80098214;
extern int D_800981FC;
extern int D_80098200;
extern int D_8009BF88[];
extern int D_8009BF8C[];
extern int D_800981E4;
extern E *D_800981E0;
extern int D_800981F0;
extern int D_800981EC;
extern int D_800981F4;
extern void (*D_800981AC)(int);
int _padInitSioMode(E *e);
void padCommandStep(E *e);
int _padVsyncHandler(void)
{
    int t;
    if (PAD_SIO->a & 2) {
        PAD_SIO->a = 0;
        return 0;
    }
    t = D_800981FC;
    D_80098214 = 1;
    if (t != 0 && D_8009BF88[0] < 150) D_8009BF88[0]++;
    if (D_80098200 == 0 && D_8009BF8C[0] < 150) D_8009BF8C[0]++;
    if (D_800981E4 != 0 && D_800981FC <= D_80098200) {
        D_800981F0 = 0;
        D_800981EC = D_800981FC;
        if (_padInitSioMode(&D_800981E0[D_800981FC]) == 0) D_800981AC(0xffff);
        D_800981F4 = 0;
        while (D_800981EC <= D_80098200) {
            padCommandStep(&D_800981E0[D_800981EC]);
        }
        PAD_SIO->e = 0x88;
    }
    return 0;
}
