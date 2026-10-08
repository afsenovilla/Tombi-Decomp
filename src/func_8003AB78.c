// FUNC 8003ab78 36 MAIN0
// MATCHING 8003ab78 36
extern unsigned char *D_8009F0F0;
extern unsigned char D_800A60A1;
void func_8003AB78(void)
{
    unsigned char *p = D_8009F0F0;
    *(int *)(p + 0x1190) = D_800A60A1;
    (*(short *)(p + 0x8a))++;
}
