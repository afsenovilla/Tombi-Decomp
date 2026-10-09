// FUNC 8002795c 792 MAIN0
// MATCHING 8002795c 792
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
    int m;   /* 0x340000 as an early local: sched2 leaves its lui last, reorg puts it in the bgez slot instead of flag=0 */

    g = &DAT_800a6038;
    if (DAT_8009d2b0 >= 3)
        return;
    flag = 0;
    k = 0x400000;
    m = 0x340000;
    v = DAT_800a604c - 0x800000;
    v -= D_1F8000F0;
    v += k;
    if (v < 0) {
        v += m;
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
