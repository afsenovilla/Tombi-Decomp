// VERSION SLPM_80165 slpm80165 8001EC14 112
#if defined(VERSION_SLPM_80165)
// Demo-only helper: stops all voices and closes the area VAB (not in areas 0 and 4).
extern void SpuSetKey(int, int);
extern void SsVabClose(short);
extern unsigned short D_8009A810;
extern short D_1F8003AE;
void func_8001EC14_slpm80165(void)
{
    short *v;
    SpuSetKey(0, 0xff0000);
    if (D_8009A810 != 0 && D_8009A810 != 4) {
        v = &D_1F8003AE;
        if (*v != -1) {
            SsVabClose(*v);
            *v = -1;
        }
    }
}
#endif
