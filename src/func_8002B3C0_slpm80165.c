// VERSION SLPM_80165 slpm80165 8002B3C0 96
#if defined(VERSION_SLPM_80165)
// Demo-only helper: runs the current area's handler on the HUD block at D_800B0508.
extern unsigned short D_8009A810;
extern unsigned char D_800B0508[], D_8009AC52;
extern int WMD_DATA;
extern void (*D_8006CE28[])(unsigned char *, int);
void func_8002B3C0_slpm80165(void)
{
    D_800B0508[3] = 0;
    D_8006CE28[D_8009A810](D_800B0508, WMD_DATA);
    D_800B0508[1] = D_8009AC52;
}
#endif
