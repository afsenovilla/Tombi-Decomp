// FUNC 80026f4c 156 MAIN0
// MATCHING 80026f4c 156
extern unsigned char DAT_8009d07f;
extern unsigned char DAT_8009c971;
extern volatile unsigned char V_8009c971 asm("DAT_8009c971");
extern unsigned char DAT_8009c970;
extern unsigned char DAT_8009d080;
extern unsigned short DAT_800a60d0;
extern unsigned short DAT_800a60d2;

void FUN_80026f4c(void)
{
    unsigned short c;
    unsigned char d;
    if (DAT_8009d07f == 0) {
        c = DAT_8009c971;
        if (c >= 8) {
            DAT_8009d080 = DAT_8009d080 + 1;
            goto done;
        }
    } else {
        c = DAT_8009c971;
        if (c >= 16) goto done;
    }
    DAT_8009c971 = c + 1;
done:
    d = V_8009c971;
    c = d;
    DAT_800a60d0 = c;
    DAT_800a60d2 = c;
    DAT_8009c970 = c;
}
