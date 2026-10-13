// FUNC 8003ab78 36 MAIN0
// MATCHING 8003ab78 36
extern unsigned char *SCRIPT_CTX;
extern unsigned char D_800A60A1;
void scriptOpGetPlayerTouchFlag(void)
{
    unsigned char *p = SCRIPT_CTX;
    *(int *)(p + 0x1190) = D_800A60A1;
    (*(short *)(p + 0x8a))++;
}
