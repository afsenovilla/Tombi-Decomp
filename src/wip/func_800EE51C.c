// FUNC 800ee51c 68 X000
// diff: game copies d into $a1 (addu a1,v0,zero in beqz delay slot) before sltiu
extern unsigned char D_8011530C[];
void func_800EE51C(unsigned char *o)
{
    unsigned char d = D_8011530C[*(short *)(o + 0xb0)] - *(int *)(o + 0x8c);
    if (d != 0) {
        if (d < 0x81) (*(int *)(o + 0x8c))--;
        else (*(int *)(o + 0x8c))++;
    }
}
