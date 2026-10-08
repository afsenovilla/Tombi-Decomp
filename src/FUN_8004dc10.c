// FUNC 8004dc10 380 MAIN0
// MATCHING 8004dc10 380
typedef struct { char p[0xc2]; unsigned short w2, w4; char q[4]; unsigned short idx; char r[2]; unsigned short wce, wd0; } O;
typedef struct { unsigned short id; short pad; short x, y; } E;
typedef struct { short n; short p[3]; } C;
extern C D_800A4648[];
extern E D_800A4DE0[][128];
extern unsigned char DAT_800a5de6[];
extern int FUN_8002df70(int, int, int);

void FUN_8004dc10(O *o, unsigned short k)
{
    int n;
    int r;
    n = D_800A4648[o->idx].n++;
    r = FUN_8002df70(o->idx, k & 0xfff, 0);
    if ((k & 0x7000) == 0x4000) {
        D_800A4DE0[o->idx][n].id = r | 0x4000;
        D_800A4DE0[o->idx][n].x = o->w2 - 0x10;
        D_800A4DE0[o->idx][n].y = o->w4 - 0x10;
    } else {
        D_800A4DE0[o->idx][n].id = r;
        D_800A4DE0[o->idx][n].x = o->wce;
        D_800A4DE0[o->idx][n].y = o->wd0;
    }
    if ((k & 0x7000) == 0x5000 || (k & 0x7000) != 0x6000)
        o->wce += DAT_800a5de6[r * 10];
}
