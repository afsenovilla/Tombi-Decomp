// FUNC 800264d0 116 MAIN0
extern struct { unsigned char on; char p[7]; short cnt; } DAT_800b1410;
extern int DAT_8009c960;
extern void FUN_80026544(void *);
void FUN_800264d0(void)
{
    if (DAT_800b1410.on != 0) {
        if (DAT_8009c960 == 6) {
            if (DAT_800b1410.cnt != 0)
                DAT_800b1410.cnt = DAT_800b1410.cnt - 1;
        } else {
            FUN_80026544(&DAT_800b1410);
        }
    }
}
