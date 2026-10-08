// FUNC 80137f54 180 X000
extern short *DAT_800a6078;
extern void FUN_801363e4(unsigned char *);
extern void FUN_801361e4(unsigned char *);
extern void FUN_80018980(void);
void FUN_80137f54(unsigned char *o)
{
    unsigned char c = o[4];
    if (c == 1) {
        if (0xd8 < DAT_800a6078[1])
            FUN_801363e4(o);
    } else if (c < 2) {
        if (c == 0)
            FUN_801361e4(o);
    } else if (c == 2) {
        o[4] = 3;
    } else if (c == 3) {
        FUN_80018980();
    }
}
