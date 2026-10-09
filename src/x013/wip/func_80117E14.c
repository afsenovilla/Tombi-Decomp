// FUNC 80117e14 532 X013
/* score 46: real function is 532 B (includes the csv entry func_80117F98, a tail split at a mid-function mult). Differs only in regs: x gets s1 instead of s3 (and ones digit s3 instead of s1), and v0/v1 swapped in the m/10%10 expansion. Tried: param types int/short, local types, statement orders, q/r temps, variable reuse for m digits, do-while priority hack.
   o15: -dl/-dg: x is 10 refs/71 insns (prio .42) > t .35 > o .31, game allocates o,t,x in the opposite order;
   short y (HI pseudo, PROMOTE_PROTOTYPES) is right (game reads a3 in the branches). 64 type combos, m%10 in other vars: no gain. */
extern void func_80118028(int, short, short);

void func_80117E14(int n, int m, int x, short y)
{
    int h, t, o, q;
    short px;

    h = n / 100;
    t = n / 10 - h * 10;
    h = h % 10;
    o = n - n / 10 * 10;
    if (h) {
        func_80118028(h, x, y);
        func_80118028(t, x + 8, y);
        func_80118028(o, x + 16, y);
        px = x + 24;
    } else if (t) {
        func_80118028(t, x, y);
        func_80118028(o, x + 8, y);
        px = x + 16;
    } else {
        func_80118028(o, x, y);
        px = x + 8;
    }
    func_80118028(10, px, y);
    px += 8;
    t = m - m / 10 * 10;
    func_80118028(m / 10 % 10, px, y);
    px += 8;
    func_80118028(t, px, y);
}
