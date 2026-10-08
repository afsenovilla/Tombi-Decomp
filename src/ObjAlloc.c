// FUNC 80018448 144 MAIN0
// MATCHING 80018448 144
// FLAGS -O2 -G0 -fno-schedule-insns2
extern short DAT_1f800238;
extern int **DAT_1f800208;
extern unsigned short DAT_1f8001c8;

int *ObjAlloc(void)
{
    char pad;
    int *o;
    int **q;
    short n = DAT_1f800238;
    char c = 3;
    if (n > 0) {
        q = DAT_1f800208;
        DAT_1f800238 = n - 1;
        DAT_1f800208 = q + 1;
        o = *q;
        *((char *)o + 0x1c) = c;
        if ((DAT_1f8001c8 & 1) == 0) {
            o[16] = (int)(o + 4);
            o[17] = (int)((char *)o + 0x18);
        } else {
            o[17] = (int)(o + 4);
            o[16] = (int)((char *)o + 0x18);
        }
        return o;
    }
    return 0;
}
