// FUNC 800187e4 84 MAIN0
extern int *DAT_1f800208;
extern unsigned short DAT_1f800238;

void ObjFree(int *o)
{
    *o = 0;
    *(o + 1) = 0;
    *(o + 2) = 0;
    *(o + 3) = 0;
    *((unsigned char *)o + 0x1c) = 0;
    *((unsigned char *)o + 0x9c) = 0;
    *((unsigned char *)o + 0x9d) = 0;
    *((unsigned char *)o + 0x9e) = 0;
    *((unsigned char *)o + 0x9f) = 0;
    DAT_1f800238++;
    *--DAT_1f800208 = (int)o;
}
