// FUNC 8006a170 144 MAIN0
extern int *ISTAT;
extern unsigned short *JOY;
extern int f();
int FUN_8006a170(void)
{
    *ISTAT = -0x81;
    if (JOY[2] & 0x80) {
        do {
            if (f()) return 0;
        } while (JOY[2] & 0x80);
    }
    JOY[5] |= 0x10;
    return 1;
}
