// FUNC 8012c6ec 436 X010
// MATCHING 8012c6ec 436
#include "TOBJ.H"

extern void *D_801322F0[];
extern unsigned short D_800A6066;
extern unsigned char D_800A603C, D_800A603D, D_800A603E, D_800A60A1;
extern short D_800A60EA, D_800A60B8, D_800A60BA, D_800A60B4;
extern unsigned char D_8009CDE3, D_8009CDE2, D_8009D07A, D_8009CE23;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);

void func_8012C6EC(TObj *o)
{
    unsigned char s = o->state;
    char pad[16];

    switch (s) {
    case 0:
        o->state = s + 1;
        o->timer = 0x1e;
        o->wac = 4;
        o->anim = D_801322F0[0];
        AnimLoadDuration(o);
    case 1:
        AnimAdvance(o);
        if (--o->timer == -1) {
            D_800A603C = 6;
            D_800A603D = 5;
            D_800A603E = 0;
            D_800A60EA = 0;
            D_800A60B8 = 0;
            D_800A60BA = 0;
            if (D_800A6066 & 1) D_800A60B4 = 0xc0;
            else D_800A60B4 = -0xc0;
            D_800A60A1 = 0;
            o->state++;
        }
        break;
    case 2:
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->b04 = 1;
        o->state = 0;
        if (D_8009CDE3 != 0xff) {
            if (D_8009D07A) o->step = 2;
            else o->step = 1;
        } else {
            if (D_8009CDE2 == 0) o->step = 3;
            else if (D_8009CDE2 == 0xff) o->step = 6;
            else if (D_8009CE23) o->step = 5;
            else o->step = 4;
        }
        break;
    }
}
