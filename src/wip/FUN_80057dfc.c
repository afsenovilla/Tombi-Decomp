// FUNC 80057dfc 224 MAIN0
extern short DAT_1f8001c6;
extern short DAT_1f800258;
extern int *DAT_1f800274;
extern int *DAT_1f800230;
extern unsigned short DAT_1f800242;
extern volatile short DAT_1f800242_s;
extern void FUN_800500c4(int);

void FUN_80057dfc(void)
{
    int n;
    int *p, *q;
    if (DAT_1f8001c6 != 0) {
        n = DAT_1f800258;
        p = DAT_1f800274;
        while (n != 0) {
            FUN_800500c4(*p++);
            n--;
        }
    } else {
        if (1) {
        DAT_1f800258 = DAT_1f800242;
        DAT_1f800274 = DAT_1f800230;
        if (DAT_1f800258 != 0) {
            do {
                q = DAT_1f800230;
                DAT_1f800230 = q + 1;
                DAT_1f800242 = DAT_1f800242 - 1;
                FUN_800500c4(*q);
            } while (DAT_1f800242_s != 0);
        }
        }
    }
}
