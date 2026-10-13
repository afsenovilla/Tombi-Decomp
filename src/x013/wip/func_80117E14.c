// FUNC 80117e14 532 X013
/* score 22 (n73 form, do-while(0) ref weighting = debt; the plain form below scores 46):
   n73: global-alloc order is o > t > x in the game (y16-copy2 first, then o=s1, t=s2, x=s3, m=s4, y=s5). Priorities
   floor_log2(refs)*refs/live: x 10/71=.42 > t 6/34=.35 > o 4/26=.31 in plain C. Weighting o/t uses with `do{..}while(0)`
   (flow counts refs * loop depth; a `do{d=o;}while(0)` copy survives cse only in the LAST block using o, else cse
   rewrites d->o and the copy dies) gives o 7 refs, t 8 refs -> the game s-regs (gen2.py: cc1 -dg prefilter on
   dispositions 79->17 78->18 74->19, then ncheck). Remaining 22: calls inside do-while get `addiu a1; move a0` instead
   of `move a0; addiu a1` (loop note pins the first in-loop insn), h%10 in a1 not v1, tail v0/v1 swap.
   Nested do-while copies do not add weight (deleted). A debt-free form needs x refs <= 8 or x live >= 97 insns.
   Original wip comment: score 46: real function is 532 B (includes the csv entry func_80117F98, a tail split at a mid-function mult). Differs only in regs: x gets s1 instead of s3 (and ones digit s3 instead of s1), and v0/v1 swapped in the m/10%10 expansion. Tried: param types int/short, local types, statement orders, q/r temps, variable reuse for m digits, do-while priority hack.
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
    int h, t, o, d, q;
    short px;

    h = n / 100;
    t = n / 10 - h * 10;
    h = h % 10;
    o = n - n / 10 * 10;
    if (h) {
        func_80118028(h, x, y);
        px = x + 8;
        do { func_80118028(t, px, y); } while (0);
        px = x + 16;
        do { func_80118028(o, px, y); } while (0);
        px = x + 24;
    } else if (t) {
        px = x;
        do { func_80118028(t, px, y); } while (0);
        do { d = o; } while (0);
        func_80118028(d, x + 8, y);
        px = x + 16;
    } else {
        do { d = o; } while (0);
        func_80118028(d, x, y);
        px = x + 8;
    }
    func_80118028(10, px, y);
    px += 8;
    t = m - m / 10 * 10;
    func_80118028(m / 10 % 10, px, y);
    px += 8;
    func_80118028(t, px, y);
}
