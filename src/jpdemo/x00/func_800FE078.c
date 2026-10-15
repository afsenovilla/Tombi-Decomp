// JPDEMO x00 800FE078 60
// Demo-only helper: on pad bits 0x60, player animation 15 and this object to step 2.
typedef struct { char p[0x2c]; unsigned short w2c; } P;
extern P *D_800893C8;
extern unsigned short D_1F8001FC;
void func_800FE078(unsigned char *o)
{
    if (D_1F8001FC & 0x60) {
        D_800893C8->w2c = 15;
        *(short *)(o + 0x7e) = 0;
        o[7] = 2;
    }
}
