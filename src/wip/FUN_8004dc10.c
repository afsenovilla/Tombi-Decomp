// FUNC 8004dc10 380 MAIN0
/* w5: score 58 (was 105) with raw (char*)+(idx<<3) post-increment, int n, short n8 = n*8. Left: game computes n*8 once
   (sll a0,s2,3 in the bne slot) and cross-jumps the final DAT_800a4de6 store; ours recomputes n8 per branch (int n8 gives 129). */
typedef struct { char p[0xc2]; unsigned short w2, w4; char q[4]; unsigned short idx; char r[2]; unsigned short wce, wd0; } O;
extern short DAT_800a4648[];
extern char DAT_800a4de0[], DAT_800a4de4[], DAT_800a4de6[];
extern unsigned char DAT_800a5de6[];
extern int FUN_8002df70(int, int, int);

void FUN_8004dc10(O *o, unsigned short k)
{
    int n; short n8;
    int r;
    n = (*(short *)((char *)DAT_800a4648 + (o->idx << 3)))++;
    r = FUN_8002df70(o->idx, k & 0xfff, 0);
    n8 = n * 8;
    if ((k & 0x7000) == 0x4000) {
        *(unsigned short *)(DAT_800a4de0 + (n8 + (o->idx << 10))) = r | 0x4000;
        *(short *)(DAT_800a4de4 + (n8 + (o->idx << 10))) = o->w2 - 0x10;
        *(short *)(DAT_800a4de6 + (n8 + (o->idx << 10))) = o->w4 - 0x10;
    } else {
        *(unsigned short *)(DAT_800a4de0 + (n8 + (o->idx << 10))) = r;
        *(short *)(DAT_800a4de4 + (n8 + (o->idx << 10))) = o->wce;
        *(short *)(DAT_800a4de6 + (n8 + (o->idx << 10))) = o->wd0;
    }
    if ((k & 0x7000) == 0x5000 || (k & 0x7000) != 0x6000)
        o->wce += DAT_800a5de6[r * 10];
}
