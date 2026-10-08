// FUNC 800ee51c 68 X000
// diff: game copies d into $a1 (addu a1,v0,zero in beqz delay slot) before sltiu; ternary/inline/copies all give score 5
extern unsigned char D_8011530C[];
static __inline__ void step(int *p, unsigned char d)
{
    if (d != 0) {
        if (d < 0x81) (*p)--;
        else (*p)++;
    }
}
void func_800EE51C(unsigned char *o)
{
    step((int *)(o + 0x8c), D_8011530C[*(short *)(o + 0xb0)] - *(int *)(o + 0x8c));
}
