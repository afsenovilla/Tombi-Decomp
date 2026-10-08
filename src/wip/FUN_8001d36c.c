// FUNC 8001d36c 324 MAIN0
extern unsigned char DAT_800a6610[];
extern void (*DAT_80077d9c[])(unsigned char *);
extern void (*DAT_80079bac[])(unsigned char *);
extern void (*DAT_80079d74[])(unsigned char *);
extern void (*DAT_8007b310[])(unsigned char *);
extern int DAT_1f800198;
/* score 16: still p/q swapped (game p=s0, q=s1 = p+2); gcc gives s0 to the la'd pointer. for/struct version makes the giv naturally but hoists &DAT_80077d9c (87). */
void FUN_8001d36c(void)
{
    unsigned char *p;
    unsigned char *q;
    void (*f)(unsigned char *);
    DAT_1f800198 = 0;
    q = DAT_800a6610 + 2;
    p = DAT_800a6610;
loop:
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
        DAT_1f800198 = DAT_1f800198 + 1;
        p += 0xd4;
        q += 0xd4;
    if (DAT_1f800198 < 200)
        goto loop;
}
