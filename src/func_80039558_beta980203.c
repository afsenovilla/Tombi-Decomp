// VERSION BETA_980203 beta980203 80039558 92
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_80037D08_slps01144.c (func_80037D08_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
void func_80039558_beta980203(unsigned char *o)
{
    switch (*(unsigned short *)(o + 0x2e)) {
    case 0: case 1: case 2: case 3:
        *(int *)(o + 0x8c) = 0; break;
    case 4: *(int *)(o + 0x8c) = 0x20; break;
    case 5: *(int *)(o + 0x8c) = 0xe0; break;
    case 6: *(int *)(o + 0x8c) = 0x40; break;
    case 7: *(int *)(o + 0x8c) = 0xc0; break;
    }
}
#endif
