// FUNC 80117e14 532 X013
/* score 46: real function is 532 B (includes the csv entry func_80117F98, a tail split at a mid-function mult). Differs only in regs: x gets s1 instead of s3 (and ones digit s3 instead of s1), and v0/v1 swapped in the m/10%10 expansion. Tried: param types int/short, local types, statement orders, q/r temps, variable reuse for m digits, do-while priority hack.
   o15: -dl/-dg: x is 10 refs/71 insns (prio .42) > t .35 > o .31, game allocates o,t,x in the opposite order;
   short y (HI pseudo, PROMOTE_PROTOTYPES) is right (game reads a3 in the branches). 64 type combos, m%10 in other vars: no gain.
   o26: game also puts h%10 in a fresh reg (v1, not h's a1) but a separate variable for it scores 76; random search of
   int/short h/t/o, digit statement orders and the m-digit variable: nothing below 46.
   n59: cause confirmed with -dg: y16 temps (sll s0,a3,16) are local-alloc'd to s0 before global alloc, so x (10 refs/71
   insns, prio .42) conflicts with s0 and takes the first new reg s1; the game needs o (4 refs/26, .31) and t (6/34, .35)
   allocated before x (global.c find_reg pass 1 = lowest unused s-reg). No source form changes x's refs (cse folds a
   running px back to x+K; short/unsigned short x is promoted and identical). Tried: q = h%10 temp, o/t multi-set forms,
   running px, px int/short, callee prototypes, digit orders: all identical code. Needs an extra real ref on o and t or a
   longer live range for x.
   n65: `short x` param with a running `x += 8` after every draw (no px) compiles to the SAME 46 bytes: gcc keeps the
   promoted a2 copy (10 refs/71 insns) apart from the HI user var (s0). Probe: an `__asm__("" :: "r"(x))` after the
   2nd tail call (x live past it, L~106, prio .28 < o .31) gives o=s1, t=s2, x=s3 (score 16); a probe before that call
   does not (50). So the game's x is live until the last digit call, but no C form found that does it without code
   (x += 0, x = x are folded/deleted before flow). Left then: hd (h%10) must be a fresh pseudo in v1: a separate `hd`
   var gets a1 by preference (set_preference takes the first operand of the minus = h, already local-alloc'd to a1);
   and the tail's v0/v1 swap in the m/10%10 expansion (local qty order). */
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
