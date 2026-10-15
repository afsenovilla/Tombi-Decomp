// FUNC 800ee6b4 260 D0SPSUB
extern unsigned short D_8009F7F2;
extern short D_801110F8, D_80111100;
extern unsigned short D_1F8001FC;

int func_800EE6B4(void)
{
    int pad = ~D_8009F7F2;
    if (pad & 0xC00) {
        if (pad & 0x10) D_801110F8 += 2;
        else if (pad & 0x40) D_801110F8 -= 2;
        else if (pad & 0x80) D_80111100 += 2;
        else if (pad & 0x20) D_80111100 -= 2;
        return -1;
    }
    if (D_1F8001FC & 0x80) return 4;
    if (D_1F8001FC & 0x20) return 0;
    if (D_1F8001FC & 0x10) return 2;
    if (D_1F8001FC & 0x40) return 6;
    return -1;
}
