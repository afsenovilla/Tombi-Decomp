// FUNC 80018790 84 MAIN0
extern int *g208;
extern unsigned short g238;
void FUN_80018790(int *o)
{
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((unsigned char *)o)[0x1c] = 0;
    ((unsigned char *)o)[0x9c] = 0;
    ((unsigned char *)o)[0x9d] = 0;
    ((unsigned char *)o)[0x9e] = 0;
    ((unsigned char *)o)[0x9f] = 0;
    g238++;
    g208 -= 1;
    g208[0] = (int)o;
}
