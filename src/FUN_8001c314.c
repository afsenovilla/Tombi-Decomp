// FUNC 8001c314 80 MAIN0
// MATCHING 8001c314 80
extern unsigned short DAT_8009c960, DAT_8009f838, DAT_8009c962;
extern unsigned char *PTR_DAT_80077b64[];
extern char *DAT_1f8001d4;

void FUN_8001c314(void)
{
    unsigned char *row = PTR_DAT_80077b64[DAT_8009c960 + DAT_8009f838];
    unsigned char b = row[DAT_8009c962];
    char *p = DAT_1f8001d4;
    *(short *)(p + 0x4e) = 0;
    *(unsigned short *)(p + 0x4c) = b;
}
