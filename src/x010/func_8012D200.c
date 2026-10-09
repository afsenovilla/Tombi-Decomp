// FUNC 8012d200 440 X010
// MATCHING 8012d200 440
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009CEAF[], D_8009D0B4[], D_8009CFC8, D_8009C959;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_8012F4EC[];
extern unsigned short Rand(void);
extern void FUN_80114968(TObj *, int, int, int, int);
extern void PoolFree_1F800210(TObj *);

void func_8012D200(TObj *o)
{
    int t;
    short *p;

    switch (o->b04) {
    case 0:
        o->w08 = 0xf0;
        o->b04++;
        break;
    case 1:
        if (D_8009C93F == 1) break;
        p = &D_1F80016A;
        if (*p >= 0xc8b) break;
        if (D_1F80016E < -0x59) break;
        if (--o->w08 != -1) break;
        o->w08 = D_8012F4EC[Rand() & 3];
        t = D_8009CEAF[0];
        if (D_8009D0B4[0] + t < 0x1e) {
            t = 5 - D_8009CFC8;
            if (t > 0 && D_8009C959 < t && (Rand() & 3) == 0)
                FUN_80114968(o, 0, *p, (short)(D_1F80016E + 0x14), D_1F800172);
        } else {
            o->b04++;
        }
        break;
    case 2:
        PoolFree_1F800210(o);
        break;
    }
}
