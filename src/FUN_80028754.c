// FUNC 80028754 368 MAIN0
// MATCHING 80028754 368
/* char *base copy of o (no hard-reg preference on a pseudo used only via raw offsets) lets n take a0 and o go to t0 like the game. */
typedef struct { char p[0x32]; short y; } TObj;
extern unsigned char DAT_800a60d6;
extern int DAT_800a606c;
extern unsigned short F2u;
extern short F2;
extern unsigned short DAT_1f80016e;
extern short E[];
extern short E2;

static __inline__ int calc(void)
{
    int t;
    if (DAT_800a60d6 == 3) {
        t = F2u - 0x14;
        return t - DAT_800a606c;
    }
    return F2u - DAT_1f80016e;
}

void FUN_80028754(TObj *o)
{
    char pad;
    short n;
    int s;
    int r;
    char *b;
    b = (char *)o;
    r = calc();
    s = r;
    r = 0;
    n = s;
    n += E[0];
    if (n != 0x3a) {
        if (n < 0x3a) {
            if (n < -6) {
                F2 += 2;
                if (*(short *)(b + 0x32) < F2) F2 = *(short *)(b + 0x32);
                else s += 2;
            } else {
                E[0] += 2;
            }
            if ((short)s + E[0] > 0x3a)
                E[0] = 0x3a - s;
        } else {
            E[0] -= 2;
            if ((short)s + E[0] < 0x3a)
                E[0] = 0x3a - s;
        }
    }
    if (F2 + E2 > *(short *)(b + 0x32))
        E2 = *(short *)(b + 0x32) - F2;
}
