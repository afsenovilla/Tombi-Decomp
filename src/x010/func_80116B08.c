// FUNC 80116b08 2140 X010
// MATCHING 80116b08 2140
/* size 2140: real start 80116B08 (missing from csv); includes the csv piece 80116D50. */
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p[3]; unsigned char n; int e[0x58]; char pad; unsigned char f165; } Q;

extern unsigned short D_8009C962;
extern unsigned char D_8009C964;
extern int D_800A4574, D_800A4570;
extern short D_1F800176, D_1F800186;
extern unsigned short D_1F8001F8;
extern unsigned char D_8012F088[];
extern int getScrollOffsetX(void);
extern int LoadImage(RECT *, void *);

void func_80116B08(Q *q, unsigned char *base)
{
    RECT r;
    short lo, hi, a, b, i, j;
    unsigned char cnt = base[0];
    int *off = (int *)(base + 4);

    switch (D_8009C962) {
    case 0:
    case 4:
        if (D_800A4574 == 0) {
            lo = (D_1F800176 + 0xa0) / 80;
            hi = lo + 5;
            a = lo + 0x30;
            b = lo + 0x37;
            if (lo < 0) lo = 0;
            if (hi >= 0x2c) hi = 0x2b;
            if (a < 0x2c) a = 0x2c;
            if (b >= cnt) b = cnt - 1;
        } else {
            lo = (D_1F800176 + getScrollOffsetX() + 0xa0) / 80;
            hi = lo + 7;
            a = lo + 0x30;
            b = lo + 0x39;
            if (lo < 0) lo = 0;
            if (hi >= 0x2c) hi = 0x2b;
            if (a < 0x2c) a = 0x2c;
            if (b >= cnt) b = cnt - 1;
        }
        if (D_8009C964 == 0) {
            for (i = lo; i <= hi; i++) {
                q->e[q->n] = (int)base + off[i];
                q->n++;
            }
        }
        for (i = a; i <= b; i++) {
            q->e[q->n] = (int)base + off[i];
            q->n++;
        }
        break;
    case 1:
    case 5:
        for (i = 0; i < 0x40; i++) {
            if (i >= cnt) break;
            q->e[q->n] = (int)base + off[i];
            q->n++;
        }
        if ((D_1F8001F8 & 7) == 0) {
            q->f165 = (q->f165 + 1) & 3;
            r.x = 0x130;
            r.y = 0x1ff;
            r.w = 0x10;
            r.h = 1;
            LoadImage(&r, D_8012F088 + q->f165 * 32);
        }
        break;
    case 2:
    case 6:
        if (D_800A4574 == 0) {
            lo = (D_1F800176 - 0x104) / 80;
            hi = lo + 9;
        } else {
            lo = (D_1F800176 + getScrollOffsetX() - 0x104) / 80;
            hi = lo + 0xa;
        }
        if (lo < 0) lo = 0;
        if (hi >= cnt) hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            q->e[q->n] = (int)base + off[i];
            q->n++;
        }
        break;
    case 3:
        if (D_1F800176 < 500) {
            lo = (D_1F800176 - 0x25) / 160;
            hi = lo + 6;
            if (lo < 0) lo = 0;
            if (hi >= 0xf) hi = 0xe;
            a = (D_1F800186 + 0x3cf) / 160;
            b = a + 5;
            if (a < 0) a = 0;
            if (b >= 10) b = 9;
        } else {
            lo = (D_1F800176 + 0x17) / 160;
            hi = lo + 5;
            if (lo < 0) lo = 0;
            if (hi >= 0xf) hi = 0xe;
            a = (D_1F800186 + 0x41f) / 160;
            b = a + 4;
            if (a < 0) a = 0;
            if (b >= 10) b = 9;
        }
        for (i = lo; i <= hi; i++) {
            for (j = a; j <= b; j++) {
                if (q->n >= 0x58) return;
                q->e[q->n] = (int)base + ((int (*)[10])off)[i][j];
                q->n++;
            }
        }
        break;
    case 7:
        if (D_1F800176 < 0x334) {
            lo = (D_1F800176 - 0x25) / 160;
            hi = lo + 6;
            if (lo < 0) lo = 0;
            if (hi >= 0xf) hi = 0xe;
            if (D_800A4570 == 0) {
                a = (D_1F800186 + 0x3cf) / 160;
                b = a + 5;
                if (a < 0) a = 0;
                if (b >= 10) b = 9;
            } else {
                a = (D_1F800186 + 0x37f) / 160;
                b = a + 6;
                if (a < 0) a = 0;
                if (b >= 10) b = 9;
            }
        } else {
            lo = (D_1F800176 + 0x67) / 160;
            hi = lo + 5;
            if (lo < 0) lo = 0;
            if (hi >= 0xf) hi = 0xe;
            a = (D_1F800186 + 0x41f) / 160;
            b = a + 4;
            if (a < 0) a = 0;
            if (b >= 10) b = 9;
        }
        for (i = lo; i <= hi; i++) {
            for (j = a; j <= b; j++) {
                if (q->n >= 0x58) return;
                q->e[q->n] = (int)base + ((int (*)[10])off)[i][j];
                q->n++;
            }
        }
        break;
    }
}
