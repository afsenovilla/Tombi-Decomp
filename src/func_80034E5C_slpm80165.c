// VERSION SLPM_80165 slpm80165 80034E5C 176
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only object step, shaped like dispatchScreenEffect: modes 0/1 run func_800331E8, modes 2/3 once set the
// byte +0x0C and clear the words +0x30/+0x32 of the object at D_800893D0.
extern unsigned char *D_800893D0;
extern void func_800331E8(void);

void func_80034E5C(unsigned char *o)
{
    switch (o[5]) {
    case 0:
        func_800331E8();
        break;
    case 1:
        func_800331E8();
        break;
    case 2:
        if (o[6] == 0) {
            D_800893D0[0xc] = 1;
            *(short *)(D_800893D0 + 0x30) = 0;
            *(short *)(D_800893D0 + 0x32) = 0;
            o[6]++;
        }
        break;
    case 3:
        if (o[6] == 0) {
            D_800893D0[0xc] = 1;
            *(short *)(D_800893D0 + 0x30) = 0;
            *(short *)(D_800893D0 + 0x32) = 0;
            o[6]++;
        }
        break;
    }
}
#endif
