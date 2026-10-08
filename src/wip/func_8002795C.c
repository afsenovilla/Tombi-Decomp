// FUNC 8002795c 792 MAIN0
// wip r8: score 8. Only diff: game emits flag=0 (move a2,zero) first after the return check and lui 0x34 fills the bgez delay slot; ours schedules flag=0 into the slot. Tried decl/stmt permutations, types, -fno-schedule-insns. w5: outer check + inline body(o,g) gives 28 (move a1,a0 in beqz slot matches) but flag=0 still lands in the bgez slot. b13: also tried statement permutations of g/flag/k/v init, flag types x test forms, flag++ and flag=0 as an inline param (all 8 or worse). b19: -da dump shows sched2 already puts flag=0 first; it is reorg (fill_simple backward scan) that pulls it into the bgez slot; the game's slot comes from fill_eager taking the fallthrough (bgez predicted not-taken). An asm barrier stops the backward scan but eager then fills from the target (andi); do-while(0)/while-break wrappers 192. b33: a real label after flag=0 (kept via static &&lab) stops the backward scan but eager still fills from the target (andi): game slot must come from a not-taken prediction for bgez (mostly_true_jump says GE 0 = taken), so the branch rtl/condition likely differs.
typedef struct O { char p0[0x18]; int b; char p1[0x30-0x1c]; short h30; short h32; char p2[0x3d-0x34];
  unsigned char b3d; unsigned char b3e; char p3[0x44-0x3f]; unsigned short h44; } O;
typedef struct G { char p0[0x14]; int d14; char p1[0x9c-0x18]; unsigned char f9c; char p2; unsigned char f9e;
  char p3[0xa9-0x9f]; unsigned char fa9; } G;
extern G DAT_800a6038;
extern signed char DAT_8009d2b0;
extern int D_1F8000F0;
extern int DAT_800a604c;
extern int F0a[], F0b[];
extern short D_1F8000F2;

void func_8002795C(O *o)
{
    G *g;
    unsigned char flag;
    int v;
    int k;

    g = &DAT_800a6038;
    if (DAT_8009d2b0 >= 3)
        return;
    flag = 0;
    k = 0x400000;
    v = DAT_800a604c - 0x800000;
    v -= D_1F8000F0;
    v += k;
    if (v < 0) {
        v += 0x340000;
        if (v >= 0) {
            o->b = 0;
            o->b3d = 1;
            o->b3e = 0;
            return;
        }
        flag = 1;
    }
    if (!flag) {
        if (o->b3e) {
            switch (g->f9e) {
            case 4:
                if (g->fa9)
                    goto clampA;
                goto clr;
            case 0:
            case 7:
            case 8:
            case 9:
            clampA:
                F0a[0] = g->d14 + k - 0x800000;
                if (D_1F8000F2 > o->h32) {
                    F0b[0] = o->h32 << 16;
                    o->b = 0;
                    o->h44 |= 8;
                }
                return;
            default:
            clr:
                o->b3e = 0;
                break;
            }
        }
        if (v > 0x10000) {
            if (v < o->b) {
                if (v < 0x40000)
                    o->b = v;
                else {
                    o->b = 0x40000;
                    o->b3e = 1;
                }
            } else {
                if (o->b < 0)
                    o->b = 0;
                o->b += 0x8000;
            }
            F0a[0] += o->b;
            if (D_1F8000F2 > o->h32) {
                F0b[0] = o->h32 << 16;
                o->b = 0;
                o->h44 |= 8;
            }
            return;
        }
        F0a[0] = g->d14 + k - 0x800000;
        if (D_1F8000F2 > o->h32) {
            F0b[0] = o->h32 << 16;
            o->b = 0;
            o->h44 |= 8;
        }
        return;
    }
    if (g->f9c == 0) {
        if (v < -0x40000) {
            if (o->b < v) {
                if (v > -0x40000)
                    goto set;
                o->b = -0x40000;
            } else {
                if (o->b > 0)
                    o->b = 0;
                o->b -= 0x4000;
            }
        } else
            goto set;
    } else {
        if (v >= -0x400000)
            return;
        v += 0x400000;
        if (v > -0x40000) {
        set:
            o->b = v;
        } else
            o->b = -0x40000;
    }
    F0a[0] += o->b;
    if (D_1F8000F2 < o->h30) {
        F0b[0] = o->h30 << 16;
        o->b = 0;
        o->h44 |= 4;
    }
}
