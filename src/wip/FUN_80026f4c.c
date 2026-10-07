// FUNC 80026f4c 156 MAIN0
extern unsigned char DAT_8009d07f;
extern unsigned char DAT_8009c971;
extern unsigned char DAT_8009c970;
extern unsigned char DAT_8009d080;
extern unsigned short DAT_800a60d0;
extern unsigned short DAT_800a60d2;

void FUN_80026f4c(void)
{
    unsigned int c;
    unsigned int d;
    if (DAT_8009d07f == 0) {
        c = DAT_8009c971;
        if (c >= 8) {
            DAT_8009d080 = DAT_8009d080 + 1;
            goto done;
        }
    } else {
        c = DAT_8009c971;
        if (c >= 16) {
            goto done;
        }
    }
    DAT_8009c971 = c + 1;
done:
    d = DAT_8009c971;
    DAT_800a60d0 = d;
    DAT_800a60d2 = d;
    DAT_8009c970 = d;
}
