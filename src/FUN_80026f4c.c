// FUNC 80026f4c 156 MAIN0
extern unsigned char DAT_8009d07f;
extern unsigned char DAT_8009c971;
extern unsigned char DAT_8009c970;
extern unsigned char DAT_8009d080;
extern unsigned short DAT_800a60d0;
extern unsigned short DAT_800a60d2;

void FUN_80026f4c(void)
{
    if (DAT_8009d07f == 0) {
        if (DAT_8009c971 >= 8) {
            DAT_8009d080 = DAT_8009d080 + 1;
            goto done;
        }
    } else if (DAT_8009c971 >= 16) {
        goto done;
    }
    DAT_8009c971 = DAT_8009c971 + 1;
done:
    DAT_800a60d0 = DAT_8009c971;
    DAT_800a60d2 = DAT_8009c971;
    DAT_8009c970 = DAT_8009c971;
}
