// JP slps01144 80017728 44
// Japanese version of PAL main0 80018154 (clearOtBuffer.c): 364 -> 268.
extern char DAT_800b0d98[];
extern void *memset(void *, int, int);
void clearOtBuffer(void)
{
    memset(DAT_800b0d98, 0, 0x10c);
}
