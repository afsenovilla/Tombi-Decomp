// FUNC 80031254 148 MAIN0
// MATCHING 80031254 148
extern void FUN_80102214(void);
extern void FUN_800e9f60(void);
extern void FUN_800eb550(void);
extern void FUN_800eab84(void);

void FUN_80031254(char *o)
{
    switch ((unsigned char)o[2]) {
    case 0:
        FUN_80102214();
        break;
    case 1:
        FUN_800e9f60();
        break;
    case 2:
        FUN_800eb550();
        break;
    case 3:
        FUN_800eab84();
        break;
    }
}
