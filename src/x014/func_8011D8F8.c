// FUNC 8011d8f8 80 X014
// MATCHING 8011d8f8 80
extern unsigned short D_1F8000EE, D_1F8000F2;

int func_8011D8F8(unsigned short *p)
{
    if ((unsigned short)(p[0] - D_1F8000EE + 0xa0) < 0x141)
        return (unsigned short)(p[1] - D_1F8000F2 + 0x6e) < 0xdd;
    return 0;
}
