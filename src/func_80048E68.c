// FUNC 80048e68 404 MAIN0
// MATCHING 80048e68 404
#define CNT (*(short *)0x1f800246)
extern short IDX;
#define LST (*(unsigned char ***)0x1f80021c)
extern unsigned char DAT_8007b6a4[];
extern short func_80047F3C(void *, void *);
extern short FUN_8004874c(void *, void *);
extern short func_80048500(void *, void *);
extern short func_800482EC(void *, void *);
extern void func_80121934(void *);
extern void func_801217F8(void *);

void func_80048E68(void)
{
    short n;
    unsigned char **p;
    unsigned char *o, *q;
    unsigned char **r;
    short v;

    n = CNT;
    p = LST;
    while (n != 0) {
        o = *p;
        p++;
        n--;
        if (o[0] != 2 && DAT_8007b6a4[o[2]] != 0) {
            r = LST;
            IDX = CNT;
            while (IDX != 0) {
                q = *r;
                IDX = IDX - 1;
                r++;
                if (q[0] != 2) {
                    switch (q[2]) {
                    case 4:
                        v = func_80047F3C(o, q);
                        if (v != 0) IDX = 0;
                        break;
                    case 6:
                        v = FUN_8004874c(o, q);
                        if (v != 0) IDX = 0;
                        break;
                    case 7:
                        v = func_80048500(o, q);
                        if (v != 0) IDX = 0;
                        break;
                    case 0xe:
                    case 0x10:
                        v = func_800482EC(o, q);
                        if (v != 0) IDX = 0;
                        break;
                    case 0x1c:
                        func_80121934(o);
                        break;
                    case 0x21:
                        func_801217F8(o);
                        break;
                    }
                }
            }
        }
    }
}
