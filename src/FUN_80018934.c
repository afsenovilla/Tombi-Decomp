// FUNC 80018934 76 MAIN0
// MATCHING 80018934 76
extern int *DAT_1f800214;
extern unsigned short DAT_1f80023c;

void FUN_80018934(volatile int *o)
{
    int *p;
    unsigned char c = ((volatile char *)o)[0x1c];
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((volatile char *)o)[0x1c] = c & 0x7f;
    DAT_1f80023c = DAT_1f80023c + 1;
    p = DAT_1f800214;
    DAT_1f800214 = p - 1;
    p[-1] = (int)o;
}
