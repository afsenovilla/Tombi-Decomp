// VERSION SLPS_01144 dspsub2 800EDFF4 156
#if defined(VERSION_SLPS_01144)
// JP memCardDrawFileLocation (PAL d0spsub2 800ec504)
extern short D_800F3080, D_800F3082, D_800F3084, D_800F3086; /* draw x, y, tpage, clut */
extern int D_800F3090;            /* draw string */
extern void *D_800F3094;          /* draw font */
extern char D_800EEE54[];         /* font */
extern int *D_800F2F44[];         /* location strings */
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern void func_800ED0E0(int a); /* memCardDrawString */

void func_800EDFF4(int tbl, int idx, int x, int y)
{
    D_800F3080 = x;
    D_800F3082 = y;
    D_800F3084 = 4;
    D_800F3086 = func_8005AD6C(0x150, 0x1F0);
    D_800F3094 = D_800EEE54;
    D_800F3090 = D_800F2F44[tbl][idx];
    func_800ED0E0(5);
}
#endif
