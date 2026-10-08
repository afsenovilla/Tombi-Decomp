// FUNC 80041240 576 MAIN0
/* score 51 (b51; was 98). Register shift fixed by an extra copy pseudo: `cc = cs; if (cc < 0) { if (k < cc) ... } else { if (cs < k) ...}`
   with int cs = (short)c, short cc, int cnt, int n, int ys = (short)y before xm (joint form/type search, 160 combos).
   Left: (1) cs = (short)c combines into `lh a2` + `move t2,a2` (game: lhu t3 kept, cs = sll/sra a2); short cs keeps lhu
   but loses the copy (77-98). (2) n should be short (game: test v0 then move t2; loop test sll/bnez) but n short = 68
   (frame 48). Old (98) notes: game has extra copy "move a1,a2" of cs before "bgez" (neg branch uses copy, else uses a2);
   tried t = cs copies, k inline per branch. */
extern unsigned short *D_1F800278;
extern unsigned short D_1F800284;
extern short *func_8003F200(int, int);

typedef struct O { char pad[0x44]; short *h; } O;

int func_80041240(O *o, short x, short y)
{
    short *p;
    int n;
    int cnt;
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
    int lo;
    int hi;
    int cs;
    short k;
    short r;
    short cc;
    char xm;
    int ys;

    p = func_8003F200((short)x, o->h[1]);
    D_1F800278 = (unsigned short *)(p + 1);
    n = *p;
    if (n == 0) return 0;
    cnt = 0;
    ys = (short)y;
    xm = x & 7;
    do {
        a = *D_1F800278++;
        n--;
        if ((a & 0xc) == 0) {
            D_1F800278 += 3;
            cnt++;
            continue;
        }
        b = *D_1F800278++;
        c = *D_1F800278++;
        cs = (short)c;
        d = *D_1F800278++;
        lo = d & 0xf;
        if (cs == 0) continue;
        hi = (d >> 4) & 0xf;
        if (cnt != 0 && (short)b + 0x10 < ys) break;
        cnt++;
        k = y - b;
        cc = cs;
        if (cc < 0) {
            if (k < cc) continue;
            if (k > 0) return 0;
        } else {
            if (cs < k) break;
            if (k < 0) continue;
        }
        D_1F800284 = (a & 0xe00) >> 9;
        if (a & 0x10) {
            if (a & 4) return 1;
            return (a >> 2) & 2;
        }
        r = xm - lo - hi * (ys - (short)b) / (short)c;
        if (a & 4) return r >= 0;
        if (a & 8) return (r < 1) << 1;
    } while (n != 0);
    return 0;
}
