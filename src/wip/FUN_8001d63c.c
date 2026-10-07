// FUNC 8001d63c 104 MAIN0
extern unsigned char DAT_80077f3c[];
extern short DAT_8007d284[];
extern char DAT_8007def0[];
extern int CdControl(int, void *, void *);
extern int CdControlF(int, void *);

void FUN_8001d63c(short a)
{
    if (CdControl(2, DAT_8007def0 + DAT_8007d284[DAT_80077f3c[a]] * 8, 0) != 0)
        CdControlF(0x15, 0);
}
