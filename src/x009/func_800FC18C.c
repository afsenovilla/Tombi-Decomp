// FUNC 800fc18c 180 X009
// MATCHING 800fc18c 180
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CE3D;
int func_800FC18C(unsigned char *o)
{
    int r = 0;
    if (D_8009D2C3 & 0x40) {
        if (*(short *)(o + 0x16) > -0x8c) {
            r = 1;
            *(short *)(o + 0x56) = -0x8c;
            if (*(short *)(o + 0x16) > -0x7c) {
                if (D_8009CE3D == 0xff) r = 2;
                else *(short *)(o + 0x16) = -0x8c;
            }
        }
    } else {
        if (*(short *)(o + 0x16) > -0x3c0) {
            r = 1;
            *(short *)(o + 0x56) = -0x3c0;
            if (*(short *)(o + 0x16) > -0x3b0) {
                if (D_8009CE3D == 0xff) r = 2;
                else *(short *)(o + 0x16) = -0x3c0;
            }
        }
    }
    return r;
}
