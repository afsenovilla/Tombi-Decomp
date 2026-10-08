// FUNC 80137f54 180 X000
// MATCHING 80137f54 180
extern short *DAT_800a6078;
extern void FUN_801363e4(unsigned char *);
extern void FUN_801361e4(unsigned char *);
extern void FUN_80018980(void);

static __inline__ int chk(void)
{
    return DAT_800a6078[1] > 0xd8;
}

void FUN_80137f54(unsigned char *o)
{
    switch (o[4]) {
    case 0:
        FUN_801361e4(o);
        break;
    case 1:
        if (chk())
            FUN_801363e4(o);
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        FUN_80018980();
        break;
    }
}
