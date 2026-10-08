// FUNC 80057288 224 MAIN0
extern short DAT_1f8001c6;
extern short DAT_1f800254;
extern short DAT_1f80024c;
extern unsigned short DAT_1f80024c_u;
extern int ** DAT_1f800224;
extern int **DAT_1f800268;
extern void FUN_800500c4(int *o);

void FUN_80057288(void)
{
    if (DAT_1f8001c6 != 0) {
        int n = DAT_1f800254;
        int **p = DAT_1f800268;
        for (; n != 0; n--) {
            int *o = *p;
            p++;
            FUN_800500c4(o);
        }
    } else {
        unsigned short n = DAT_1f80024c_u;
        int **q = DAT_1f800224;
        DAT_1f800254 = n;
        DAT_1f800268 = q;
        if (n != 0) do {
            int **p = DAT_1f800224;
            DAT_1f800224 = p + 1;
            DAT_1f80024c_u = DAT_1f80024c_u - 1;
            FUN_800500c4(*p);
        } while (DAT_1f80024c != 0);
    }
}
