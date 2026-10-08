// FUNC 801176d8 1492 X000
/* wip (ncheck score 212). Spans splat entries 801176D8+80117928 (jtbl 80115EF8, cases 0,1,2,4/5).
   Logic should be right; fails on register choice in the x/80 divisions: game puts x in v1 and the
   0x66666667 magic in v0 (a0 after getScrollOffsetX), with mult in each branch and the sra/mfhi tail
   cross-jumped; also hi gets an extra move copy (move a3). */
typedef struct { char pad[3]; unsigned char n; unsigned char *p[1]; } L;
extern unsigned short D_8009C962;
extern unsigned char D_8009CDA2;
extern int D_800A4574;
extern short D_1F800176;
extern unsigned short D_1F800176u;
extern short D_1F800186;
extern int getScrollOffsetX(void);

void func_801176D8(L *o, unsigned char *base)
{
    unsigned char cnt = base[0];
    int *tbl = (int *)(base + 4);
    short lo, hi, i, v;
    int x;

    switch (D_8009C962) {
    case 0:
        if (D_8009CDA2)
            { x = D_1F800176 + 0x8c; v = x / 80; }
        else if (D_800A4574 == 0)
            { x = D_1F800176 + 0x64; v = x / 80; }
        else
            { x = D_1F800176 + getScrollOffsetX() + 0x64; v = x / 80; }
        lo = v;
        hi = v + 8;
        if (lo <= 0)
            lo = 1;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    case 1:
        if (D_8009CDA2)
            { x = D_1F800176 + 0x3c; v = x / 80; }
        else if (D_800A4574 == 0)
            { x = D_1F800176 + 0x3c; v = x / 80; }
        else
            { x = D_1F800176 + getScrollOffsetX() + 0x3c; v = x / 80; }
        lo = v;
        hi = v + 9;
        if (lo <= 0)
            lo = 1;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    case 2:
        if (D_800A4574 == 0)
            { x = D_1F800176 + 0x8c; v = x / 80; }
        else
            { x = D_1F800176 + getScrollOffsetX() + 0x8c; v = x / 80; }
        lo = v;
        hi = v + 8;
        if (lo <= 0)
            lo = 1;
        if (hi >= 42)
            hi = 41;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        lo = (D_1F800186 + 0x550) / 80;
        if (lo < 0)
            lo = 0;
        lo += 42;
        hi = lo + 7;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    case 4:
    case 5:
        if (D_8009CDA2) {
            for (i = 0; i < cnt; i++) {
                o->p[i] = base + tbl[i];
                o->n++;
            }
        } else {
            if (D_800A4574 == 0)
                { x = (short)D_1F800176u; v = x / 80; }
            else
                { x = D_1F800176 + getScrollOffsetX(); v = x / 80; }
            lo = v;
            hi = v + 8;
        }
        if (lo < 0)
            lo = 0;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    }
}
