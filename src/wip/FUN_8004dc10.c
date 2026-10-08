// FUNC 8004dc10 380 MAIN0
typedef struct { char p[0xc2]; unsigned short w2, w4; char q[6]; unsigned short idx; char r[2]; unsigned short wce, wd0; } O;
extern short DAT_800a4648[];
extern char DAT_800a4de0[], DAT_800a4de4[], DAT_800a4de6[];
extern unsigned char DAT_800a5de6[];
extern int FUN_8002df70(int, int, int);

void FUN_8004dc10(volatile O *o, unsigned int k)
{
    unsigned short u;
    short n, s;
    int r, m;
    n = DAT_800a4648[o->idx * 4];
    DAT_800a4648[o->idx * 4] = n + 1;
    r = FUN_8002df70(o->idx, k & 0xfff, 0);
    m = n * 8;
    if ((k & 0x7000) == 0x4000) {
        *(unsigned short *)(DAT_800a4de0 + m + o->idx * 0x400) = r | 0x4000;
        *(short *)(DAT_800a4de4 + m + o->idx * 0x400) = o->w2 - 0x10;
        u = o->idx;
        s = o->w4 - 0x10;
    } else {
        *(unsigned short *)(DAT_800a4de0 + m + o->idx * 0x400) = r;
        *(short *)(DAT_800a4de4 + m + o->idx * 0x400) = o->wce;
        u = o->idx;
        s = o->wd0;
    }
    *(short *)(DAT_800a4de6 + m + u * 0x400) = s;
    if ((k & 0x7000) == 0x5000 || (k & 0x7000) != 0x6000)
        o->wce = o->wce + DAT_800a5de6[r * 10];
}
