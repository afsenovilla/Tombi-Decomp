// FUNC 8011116c 464 X000
/* diff: game keeps o in a2 (move a2,a0), recomputes c1<<2 for the |3 branch (no CSE); rest of the body matches structurally */
typedef struct {
    short t0, t1, t2;
    unsigned char pad[0x6c - 6];
    unsigned short d0, d1, d2, d3;
} T;
extern T D_80115510[];
extern unsigned char D_8009D2B3;
extern unsigned char D_8009C990;
extern unsigned char D_8009CF06;
extern unsigned char D_8009D006;
static __inline__ int getidx(unsigned char c)
{
    int i = D_8009D2B3 + (c << 2);
    if (D_8009C990 & 3) i = (c << 2) | 3;
    if (D_8009CF06) i = 8;
    if (D_8009D006) i = 8;
    return i;
}
void func_8011116C(unsigned char *o)
{
    int i;
    unsigned short x;
    short s;

    i = getidx(o[0xc1]);
    x = *(unsigned short *)(o + 0xb2);
    if ((unsigned short)(x + 0x50) <= 0xa0) {
        *(short *)(o + 0xb2) = 0;
        return;
    }
    s = x;
    if (D_80115510[i].t2 < s) x -= D_80115510[i].d0;
    else if (D_80115510[i].t1 < s) x -= D_80115510[i].d1;
    else if (D_80115510[i].t0 < s) x -= D_80115510[i].d2;
    else if (s > 0) x -= D_80115510[i].d3;
    else if (s < -D_80115510[i].t2) x += D_80115510[i].d0;
    else if (s < -D_80115510[i].t1) x += D_80115510[i].d1;
    else if (s < -D_80115510[i].t0) x += D_80115510[i].d2;
    else if (s < 0) x += D_80115510[i].d3;
    else return;
    *(unsigned short *)(o + 0xb2) = x;
}
