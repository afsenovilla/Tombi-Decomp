// FUNC 8012284c 608 X014
// MATCHING 8012284c 608
#include "TOBJ.H"
typedef struct { short x, y, w4, w6; } PT;

extern unsigned char D_8009C942;
extern unsigned short D_8009C962;
extern PT D_801265D4[], D_80126654[];
extern unsigned short D_1F8000EE, D_1F8000F2;
extern int Rand(void);
extern TObj *FUN_80018448(void);
extern void func_80118314(int, int, int);
extern void playSFX(int);

void func_8012284C(TObj *o)
{
    PT *p;
    int i, n, in, k;
    int x, y;

    if (D_8009C942 == 1) return;
    p = D_801265D4;
    if (D_8009C962 == 7) p = D_80126654;
    o->wb0 = 0;
    n = 0;
    k = 0;
loop:
    in = 0;
    if ((unsigned short)(p->x - D_1F8000EE + 0x9e) < 0x13d)
        in = (unsigned short)(p->y - D_1F8000F2 + 0x46) < 0xab;
    p++;
    n += in;
    o->wb0 |= in << k;
    k++;
    if (k < 16) goto loop;
    if (n) {
        in = 0;
        for (i = 0; i < 8; i++) {
            k = Rand() & 0xf;
            if ((o->wb0 >> k) & 1) {
                o->wae = k;
                in = 1;
                break;
            }
        }
        if (!in) {
            for (i = 0; i < 16; i++) {
                if ((o->wb0 >> i) & 1) {
                    o->wae = i;
                    break;
                }
            }
        }
    } else {
        o->wae = 0;
    }
    p = D_801265D4;
    if (D_8009C962 == 7) p = D_80126654;
    p += o->wae;
    o->a.p.whole = o->velX = p->x;
    o->velY = p->y;
    o->wac = p->w4;
    o->step++;
    x = p->x << 16;
    y = p->y << 16;
    for (in = 0; in < 2; in++) {
        TObj *e = FUN_80018448();
        if (e) {
            e->active = 2;
            e->type = 0x25;
            e->animFrame = in & 1;
            e->a.raw = x;
            e->y.raw = y;
            e->b.raw = 0;
        }
    }
    func_80118314(p->x, p->y, 0);
    if (D_8009C962 == 7) playSFX(0xf5);
    else playSFX(0xe9);
}
