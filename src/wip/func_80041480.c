// FUNC 80041480 568 MAIN0
// Score 54 (inline body(unsigned char x, int y)): game hoists x&7 into t6 as a single andi in the preheader after (short)y, and keeps x in s1/y copy in s2; ours adds andi 0xff and frame/regs differ.
typedef struct {
    char pad[0x44];
    short *p44;
} O80041480;

extern unsigned short *func_8003F200(int, int);
extern unsigned short *D_1F800278;
#define D D_1F800278

static __inline__ int body(unsigned char x, int y)
{
    short n;
    int cnt;
    unsigned short f, a, b, c;
    short bs, d, r;
    int lo, w;

    n = *D++;
    if (n == 0) {
        return 0;
    }
    cnt = 0;
    do {
        f = *D++;
        n--;
        if ((f & 0xc) == 0 || (f & 0x10)) {
            D += 3;
            cnt++;
            continue;
        }
        a = *D++;
        b = *D++;
        c = *D++;
        bs = b;
        lo = c & 0xf;
        if (bs == 0) {
            continue;
        }
        w = (c >> 4) & 0xf;
        if (cnt != 0 && (short)a + 16 < (short)y) {
            return 0;
        }
        cnt++;
        d = y - a;
        if (bs < 0) {
            if (d < bs) {
                continue;
            }
            if (d > 0) {
                return 0;
            }
        } else {
            if (bs < d) {
                return 0;
            }
            if (d < 0) {
                continue;
            }
        }
        if (f & 0x10) {
            if (f & 4) {
                return 1;
            }
            return (f >> 2) & 2;
        }
        r = (x & 7) - lo - w * ((short)y - (short)a) / (short)b;
        if (f & 4) {
            return r >= 0;
        }
        if (f & 8) {
            return (r < 1) << 1;
        }
    } while (n != 0);
    return 0;
}

int func_80041480(O80041480 *o, short x, short y)
{
    D = func_8003F200(x, o->p44[1]);
    return body(x, y);
}
