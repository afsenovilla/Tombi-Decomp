// FUNC 8001e288 68 MAIN0
// MATCHING 8001e288 68
extern unsigned short DAT_800a34b0;
extern unsigned short DAT_8009d690;
int FUN_8001e288(unsigned short a)
{
    switch (a & 0xf00) {
    case 0:
        DAT_800a34b0 = a & 0xff;
        break;
    case 0x100:
        DAT_8009d690 = a & 0xff;
        break;
    }
    return 0;
}
