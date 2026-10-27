// FUNC 8006a170 144 MAIN0
// MATCHING 8006a170 144
extern int *D_8009820C;
extern unsigned short *PAD_SIO;
extern int chkRC2wait();

int _padClrIntSio0(void)
{
    int *is = D_8009820C;
    unsigned short *j = PAD_SIO;
    *is = -0x81;
    if (j[2] & 0x80) {
        do {
            if (chkRC2wait()) return 0;
        } while (PAD_SIO[2] & 0x80);
    }
    PAD_SIO[5] |= 0x10;
    return 1;
}
