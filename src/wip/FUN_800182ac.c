// FUNC 800182ac 128 MAIN0
extern short DAT_1f800236;
extern int **DAT_1f800204;
extern unsigned short DAT_1f8001c8;

int FUN_800182ac(void)
{
    int *o;
    short n = DAT_1f800236;
    if (n > 0) {
        DAT_1f800236 = n - 1;
        o = *DAT_1f800204++;
        if ((DAT_1f8001c8 & 1) == 0) {
            o[0x10] = (int)(o + 4);
            o[0x11] = (int)(o + 6);
        } else {
            o[0x11] = (int)(o + 4);
            o[0x10] = (int)(o + 6);
        }
    } else {
        o = 0;
    }
    return (int)o;
}
