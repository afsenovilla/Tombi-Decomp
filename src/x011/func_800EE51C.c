// FUNC 800ee51c 68 X011
// MATCHING 800ee51c 68
extern unsigned char D_8011530C[];
void func_800EE51C(unsigned char *o)
{
    int t = (unsigned char)(D_8011530C[*(short *)(o + 0xb0)] - *(int *)(o + 0x8c));
    unsigned char d = t;
    if (t != 0) {
        if (d < 0x81) (*(int *)(o + 0x8c))--;
        else (*(int *)(o + 0x8c))++;
    }
}
