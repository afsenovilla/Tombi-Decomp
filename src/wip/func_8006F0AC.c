// FUNC 8006f0ac 52 MAIN0
// game computes the array address in $v0 (lui v0; addu v0,v0,a0; lw v0,lo(v0)); ours goes through $at
extern int *D_800982C8;
extern int D_800982D0[];
int func_8006F0AC(unsigned short i)
{
    D_800982C8[1] &= ~D_800982D0[i];
    return 1;
}
