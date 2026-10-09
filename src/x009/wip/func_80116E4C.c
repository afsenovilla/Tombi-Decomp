/* score 119: structure, calls and loops match (switch needs the empty cases 1-5). Writing the first lo-block and the first branch of the last block as `a = div; if (a < 0) a = 0; a += K; hi = a + 8;` keeps hi = a + 8 unfolded (was lo + 0x24). Remaining: register choice, a+K and hi swapped (game a2/v1, ours v1/a2), (D_1F800176 - 0x668)/80 branch dividend in v0 not v1 so the mult is not cross-jumped, lo/hi clamps. Game copies i/hi into a1/a0 before each loop (move a1,a0; move a0,v1) like inline param copies, but a range() inline swaps the loop regs. Tried: int/short/ushort loop vars, int/u8/short cnt, separate getScrollOffsetX temp, inverted ifs, declaration orders. */
// FUNC 80116e4c 1560 X009
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p[3]; unsigned char n; int e[0x58]; unsigned char b164; } Q;

extern unsigned short D_8009C962;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CDA2;
extern unsigned short D_8009CD94;
extern unsigned char D_8009C964;
extern unsigned char D_8009C93A;
extern int D_800A4574;
extern short D_1F800176;
extern unsigned long D_8012A858[];
extern int getScrollOffsetX(void);
extern int LoadImage(RECT *, unsigned long *);
extern int MoveImage(RECT *, int, int);

#define PUSH(i)                                 \
    q->e[q->n] = (int)base + off[i];            \
    q->n++;

void func_80116E4C(Q *q, unsigned char *base)
{
    RECT r;
    short lo, hi, a, i;
    unsigned char cnt = base[0];
    int *off = (int *)(base + 4);

    switch (D_8009C962) {
    case 0:
        if ((D_8009D2C3 & 1) && q->b164 == 0) {
            r.x = 0xb0;
            r.y = 0x1f0;
            r.w = 0x10;
            r.h = 1;
            LoadImage(&r, D_8012A858);
            r.x = 0xa;
            r.y = 0x170;
            r.w = 0x16;
            r.h = 0x40;
            MoveImage(&r, 0x2a, 0x140);
            r.x = 0x20;
            r.y = 0x110;
            r.w = 0x16;
            r.h = 0x10;
            MoveImage(&r, 0x20, 0x100);
            q->b164++;
        }
        if (D_8009CDA2) {
            if (D_8009CD94 == 3) {
                hi = cnt - 1;
                for (i = cnt - 0x10; i <= hi; i++) {
                    PUSH(i)
                }
            } else if (D_8009CD94 == 1) {
                hi = cnt - 4;
                for (i = cnt - 0x14; i <= hi; i++) {
                    PUSH(i)
                }
                hi = 0x1b;
                for (i = 0x13; i <= hi; i++) {
                    PUSH(i)
                }
            } else {
                a = (D_1F800176 - 0x384) / 80;
                if (a < 0) a = 0;
                a += 0x1c;
                hi = a + 8;
                if (hi >= cnt) hi = cnt - 1;
                for (i = a; i <= hi; i++) {
                    PUSH(i)
                }
            }
        }
        if (D_8009C964 == 0x20 || D_8009C93A == 0) {
            if (D_800A4574 == 0)
                lo = (D_1F800176 - 0x668) / 80;
            else
                lo = (D_1F800176 + getScrollOffsetX() - 0x668) / 80;
            hi = lo + 8;
            if (lo < 0) lo = 0;
            if (hi >= 0x1d) hi = 0x1c;
            for (i = lo; i <= hi; i++) {
                PUSH(i)
            }
        }
        if (D_800A4574 == 0) {
            a = (D_1F800176 - 0x3ac) / 80;
            if (a < 0) a = 0;
            a += 0x1c;
            hi = a + 8;
        } else {
            lo = (D_1F800176 + getScrollOffsetX() - 0x3ac) / 80;
            if (lo < 0) lo = 0;
            a = lo + 0x1b;
            hi = a + 10;
        }
        if (hi >= cnt) hi = cnt - 1;
        for (i = a; i <= hi; i++) {
            PUSH(i)
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        for (i = 0; i < 0x40 && i < cnt; i++) {
            PUSH(i)
        }
        break;
    }
}
