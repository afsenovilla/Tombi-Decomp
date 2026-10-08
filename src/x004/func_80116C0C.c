// FUNC 80116c0c 304 X004
// MATCHING 80116c0c 304
#include "TOBJ.H"

extern short D_1F80016A, D_1F800172;
extern unsigned short D_1F8001FC;
extern unsigned char D_800A60E4, D_800A60A1, D_800A60D4, D_800A60D6, D_800A60E2, D_8009C940;
extern unsigned char D_8009D2B0, D_8009C93F, D_8009C942;
extern int FUN_800270a0(TObj *, int, int);

static __inline__ short Blocked(void)
{
    if (D_800A60E4 >= 2) return 1;
    if (!D_800A60A1) return 1;
    if (D_8009C940 | (D_800A60E2 | (D_800A60D4 | D_800A60D6))) return 1;
    if (D_1F8001FC & 0x10) return 0;
    return 1;
}

void func_80116C0C(TObj *o)
{
    if (D_1F80016A < 0x78 && D_1F800172 == 0x546) {
        if (FUN_800270a0(o, 1, 0)) {
            D_8009D2B0 = 0;
            D_8009C93F = 1;
            D_8009C942 = 1;
        }
        return;
    }
    if (Blocked()) return;
    if (D_1F800172 == 0x4ec && (unsigned short)D_1F80016A - 0x408 < 0x60u) {
        if (FUN_800270a0(o, 1, 2)) {
            D_8009D2B0 = 0;
            D_8009C93F = 1;
            D_8009C942 = 1;
        }
    }
}
