// FUNC 800310d0 388 MAIN0
typedef struct S { unsigned char a, p, b; } S;
extern S DAT_a;
extern unsigned char DAT_603a;
extern unsigned short DAT_960, DAT_962;
extern unsigned char DAT_36c2;
extern void FUN_80031254(void);
extern void FUN_8003bd80(void);

void FUN_800310d0(void)
{
    unsigned char v;
    S *p = &DAT_a;
    DAT_603a = 0;
    switch (DAT_960) {
    case 2:
        switch (DAT_962) {
        case 0: v = 1; break;
        case 3: v = 3; break;
        default: goto end;
        }
        DAT_603a = v;
        break;
    case 5:
    case 8:
        if (DAT_962 == 0 || DAT_962 == 2)
            p->b = 2;
        break;
    case 0x13:
        switch (DAT_962) {
        case 0: v = 1; break;
        case 2: v = 2; break;
        default: goto end;
        }
        DAT_603a = v;
        break;
    }
end:
    if ((DAT_960 != 6 || DAT_962 != 0) && (DAT_960 != 9 || DAT_962 < 6)) {
        if (p->a != 0)
            FUN_80031254();
        if (DAT_962 != 7 || DAT_36c2 == 0)
            FUN_8003bd80();
    }
}
