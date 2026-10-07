// FUNC 8001e368 200 MAIN0
// MATCHING 8001e368 200
extern short DAT_8009bd38;
extern short DAT_8009bd34;
extern short DAT_8009bd30;
extern short DAT_8009bd3c;
extern short DAT_8009bd2c;
extern short DAT_8009bd40;
extern int DAT_8009bd08;

int FUN_8001e368(unsigned short a)
{
    switch (a & 0xf00) {
    case 0:
        DAT_8009bd34 = a & 0xff;
        DAT_8009bd30 = 1;
        break;
    case 0x100:
        DAT_8009bd38 = a & 0xff;
        if (DAT_8009bd38 >= 0x80)
            DAT_8009bd38 = DAT_8009bd38 | 0xff00;
        break;
    case 0x200:
        DAT_8009bd3c = a & 0xff;
        break;
    case 0x300:
        DAT_8009bd2c = 1;
        DAT_8009bd40 = DAT_8009bd08;
        break;
    }
    return 0;
}
