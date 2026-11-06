// VERSION SLPS_01144 x09 8011C484 56
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
extern unsigned char D_8009EBA0;
extern void func_80031C3C(int, int, int, int);

void func_8011C484_slps01144(short *o)
{
    if (D_8009EBA0 != 0) {
        func_80031C3C(0, o[9], o[11], o[13]);
    }
}
#endif
