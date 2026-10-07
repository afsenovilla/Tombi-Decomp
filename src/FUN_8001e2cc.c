// FUNC 8001e2cc 156 MAIN0
// MATCHING 8001e2cc 156
extern short DAT_8009bd20;
extern short DAT_8009bd1c;
extern short DAT_8009bd18;
extern short DAT_8009bd28;

int FUN_8001e2cc(unsigned short a)
{
    switch (a & 0xf00) {
    case 0:
        DAT_8009bd1c = a & 0xff;
        DAT_8009bd18 = 1;
        break;
    case 0x100:
        DAT_8009bd20 = a & 0xff;
        if (DAT_8009bd20 >= 0x80)
            DAT_8009bd20 = DAT_8009bd20 | 0xff00;
        break;
    case 0x200:
        DAT_8009bd28 = a & 0xff;
        break;
    }
    return 0;
}
