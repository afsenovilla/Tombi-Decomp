// FUNC 800187e4 84 MAIN0
// MATCHING 800187e4 84
// FLAGS -O2 -G0
extern int *DAT_1f800208;
extern unsigned short DAT_1f800238;

void ObjFree(volatile int *o)
{
    int *p;
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((volatile char *)o)[0x1c] = 0;
    ((volatile char *)o)[0x9c] = 0;
    ((volatile char *)o)[0x9d] = 0;
    ((volatile char *)o)[0x9e] = 0;
    ((volatile char *)o)[0x9f] = 0;
    DAT_1f800238 = DAT_1f800238 + 1;
    p = DAT_1f800208;
    DAT_1f800208 = p - 1;
    p[-1] = (int)o;
}
