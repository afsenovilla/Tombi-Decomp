/* score 131: structure, calls and loops match (switch needs the empty cases 1-5); remaining: register choice in the (D_1F800176 - K) / 80 branches (dividend in v0 instead of v1 so the mult is not cross-jumped) and in the lo/hi clamps. Tried: int/short/ushort loop vars, int/u8/short cnt, separate getScrollOffsetX temp, inverted ifs */
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
                lo = (D_1F800176 - 0x384) / 80;
                if (lo < 0) lo = 0;
                a = lo + 0x1c;
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
            lo = (D_1F800176 - 0x3ac) / 80;
            if (lo < 0) lo = 0;
            a = lo + 0x1c;
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
