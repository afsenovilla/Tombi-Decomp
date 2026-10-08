// FUNC 8001d36c 324 MAIN0
extern unsigned char DAT_800a6610[];
#define DAT_80077d9c ((void (**)(unsigned char *))0x80077d9c)
#define DAT_80079bac ((void (**)(unsigned char *))0x80079bac)
#define DAT_80079d74 ((void (**)(unsigned char *))0x80079d74)
#define DAT_8007b310 ((void (**)(unsigned char *))0x8007b310)
extern int DAT_1f800198;

void FUN_8001d36c(void)
{

    unsigned char *p = DAT_800a6610;
    unsigned char *q;
    void (*f)(unsigned char *);
    DAT_1f800198 = 0;
    q = p + 2;
    do {
        if (*p != 0) {
            switch (q[0x1a] & 0x7f) {
            case 2: f = DAT_80077d9c[*q]; break;
            case 3: f = DAT_80079bac[*q]; break;
            case 4: f = DAT_80079d74[*q]; break;
            case 5: f = DAT_8007b310[*q]; break;
            default: goto next;
            }
            f(p);
        }
    next:
        q += 0xd4;
        DAT_1f800198 = DAT_1f800198 + 1;
        p += 0xd4;
    } while (DAT_1f800198 < 200);
}
