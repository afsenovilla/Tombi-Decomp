// VERSION SLPS_01144 x14 80117B54 976
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern short D_80122EA8[];
extern unsigned short D_80122E98[];
extern short D_80122EC4, D_80122EC6;
extern void *D_80124DCC;
extern int D_1F8002DCx[];
extern short D_80077150[], D_80076D50[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void func_801179F0_slps01144(TObj *);
extern void func_801170C4(TObj *);

void func_80117B54_slps01144(TObj *o)
{
    TObj *p;
    int v;
    int c, sn;
    int n;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        v = D_80122EA8[((unsigned short*)&GAME.selectedSection)[0]];
        o->b0a = 9;
        o->w1e = 8;
        o->d8c = v;
        o->ba6 = 0;
        o->ba5 = 0;
        o->ba7 = 0;
        o->b6a = 0;
        o->b0d = 1;
        o->d38 = v;
        o->d30 = o->d8c;
        o->w08 = (D_80122E98[((unsigned short*)&GAME.selectedSection)[0]] << 6) | 0xf;
        o->d3c = D_1F8002DCx[0];
        o->anim = D_80124DCC;
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (((unsigned short*)&GAME.selectedSection)[0]) {
        case 0:
            p = (*(TObj **)&GVARS.p54);
            if (!o->b6a && p) {
                o->d8c = ((0x1000 - p->d8c) >> 4) & 0xff;
                o->h->p.whole = p->a.p.whole;
                o->y.p.whole = p->y.p.whole;
            }
            break;
        case 2:
            p = (*(TObj **)&GVARS.p54);
            if (!o->b6a && p) {
                {
                    int i = (o->d30 + 1) & 0xff;
                    o->d30 = i;
                    c = D_80077150[i];
                    sn = D_80076D50[i];
                }
                o->h->raw = p->h->raw + c * 0x700;
                o->y.raw = p->y.raw + sn * 0x700;
                o->d8c = (o->d30 + 0x80) & 0xff;
            }
            break;
        case 3:
            if (!o->b6a) o->d8c = (o->d8c - 1) & 0xff;
            break;
        case 4:
            if (!o->b6a) o->d8c = (o->d8c + 1) & 0xff;
            break;
        case 5:
            if (!o->b6a) {
                o->d8c = (o->d8c + 1) & 0xff;
                func_801179F0_slps01144(o);
            }
            break;
        case 6:
            if (!o->b6a) {
                o->d8c = (o->d8c + 1) & 0xff;
                switch (o->substep) {
                case 0:
                    *(short *)((char *)o + 0x74) = D_80122EC4;
                    o->w76 = D_80122EC6;
                    o->substep++;
                    break;
                case 1:
                    if (++o->a.p.whole > o->w74) {
                        o->a.p.whole = o->w74;
                        n = o->substep + 1;
                        goto set;
                    }
                    break;
                case 2:
                    if (--o->a.p.whole < o->w76) {
                        n = 1;
                        o->a.p.whole = o->w76;
                    set:
                        o->substep = n;
                    }
                    break;
                }
            }
            break;
        case 1:
        case 7:
            if (!o->b6a) func_801179F0_slps01144(o);
            break;
        default:
            return;
        }
        func_801170C4(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
