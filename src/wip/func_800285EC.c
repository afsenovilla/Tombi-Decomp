// FUNC 800285ec 360 MAIN0
/* score 39 (ncheck): only missing `move a2,v0` (calc result copied into s) after the E address load */
typedef struct { char p[0x30]; short y; } TObj;
extern unsigned char DAT_800a60d6;
extern int DAT_800a606c;
extern unsigned short F2u;
extern short F2;
extern struct { short v; } F2s;
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

void func_800285EC(TObj *o)
{
    char pad;
    short n;
    int s = calc();
    n = E[0] + s;
    if (n != -0x50) {
        if (n > -0x50) {
            if (n >= -0x33) {
                F2 -= 2;
                if (F2 < *(short *)((char *)o + 0x30)) F2 = *(short *)((char *)o + 0x30);
                else s -= 2;
            } else {
                E[0] -= 2;
            }
            if ((short)s + E[0] < -0x50)
                E[0] = -0x50 - s;
        } else {
            E[0] += 2;
            if ((short)s + E[0] > -0x50)
                E[0] = -0x50 - s;
        }
    }
    if (F2 + E2 < o->y)
        E2 = o->y - F2;
}

