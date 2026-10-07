// FUNC 80018448 144 MAIN0
extern short DAT_1f800238;
extern int **DAT_1f800208;
extern unsigned short DAT_1f8001c8;

int *ObjAlloc(void)
{
    int pad;
    int *r;
    int *o;
    if (DAT_1f800238 < 1) {
        r = 0;
    } else {
        DAT_1f800238 = DAT_1f800238 - 1;
        o = *DAT_1f800208;
        DAT_1f800208 = DAT_1f800208 + 1;
        *((char *)o + 0x1c) = 3;
        if ((DAT_1f8001c8 & 1) == 0) {
            o[16] = (int)(o + 4);
            o[17] = (int)((char *)o + 0x18);
        } else {
            o[17] = (int)(o + 4);
            o[16] = (int)((char *)o + 0x18);
        }
        r = o;
    }
    return r;
}
