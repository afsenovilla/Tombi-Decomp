// FUNC 8006f0ac 52 MAIN0
// MATCHING 8006f0ac 52
// FLAGS -O2 -G0
// AS gnu
// I_MASK &= ~bit (libapi, built by a newer compiler): GNU as keeps the symbol+index macro in $v0.
extern int *D_800982C8;
static int D_800982D0[6] = {
    16, 32, 64, 1,
};
int intrMaskClearBit(unsigned short i)
{
    D_800982C8[1] &= ~D_800982D0[i];
    return 1;
}
