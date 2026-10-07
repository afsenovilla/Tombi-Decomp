// FUNC 80018934 76 MAIN0
extern int *DAT_1f800214;
extern unsigned short DAT_1f80023c;

void FUN_80018934(int *o)
{
    *o = 0;
    *(o + 1) = 0;
    *(o + 2) = 0;
    *(o + 3) = 0;
    *((unsigned char *)o + 0x1c) &= 0x7f;
    DAT_1f80023c++;
    *--DAT_1f800214 = (int)o;
}
