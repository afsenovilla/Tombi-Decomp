// FUNC 80048e68 404 MAIN0
extern short DAT_1f800246;
extern short DAT_1f80019e;
extern unsigned char **DAT_1f80021c;
extern unsigned char DAT_8007b6a4[];
extern short FUN_80047f3c(void *, void *);
extern short FUN_8004874c(void *, void *);
extern short FUN_80048500(void *, void *);
extern short FUN_800482ec(void *, void *);
extern void FUN_80121934(void *);
extern void FUN_801217f8(void *);

void FUN_80048e68(void)
{
    int n;
    unsigned char **p;
    unsigned char *o, *q;
    unsigned char **r;
    short v;
    v = DAT_1f800246;
    n = v;
    p = DAT_1f80021c;
    if (v != 0) do {
        o = *p;
        p++;
        n--;
        if (o[0] != 2 && DAT_8007b6a4[o[2]] != 0) {
            r = DAT_1f80021c;
            DAT_1f80019e = DAT_1f800246;
            while (DAT_1f80019e != 0) {
                q = *r;
                DAT_1f80019e = DAT_1f80019e - 1;
                r++;
                if (q[0] != 2) {
                    switch (q[2]) {
                    case 4:
                        v = FUN_80047f3c(o, q);
                        if (v != 0) DAT_1f80019e = 0;
                        break;
                    case 6:
                        v = FUN_8004874c(o, q);
                        if (v != 0) DAT_1f80019e = 0;
                        break;
                    case 7:
                        v = FUN_80048500(o, q);
                        if (v != 0) DAT_1f80019e = 0;
                        break;
                    case 0xe:
                    case 0x10:
                        v = FUN_800482ec(o, q);
                        if (v != 0) DAT_1f80019e = 0;
                        break;
                    case 0x1c:
                        FUN_80121934(o);
                        break;
                    case 0x21:
                        FUN_801217f8(o);
                        break;
                    }
                }
            }
        }
    } while ((n & 0xffff) != 0);
}
