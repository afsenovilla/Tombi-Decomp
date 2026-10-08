// FUNC 8002c238 516 MAIN0
// MATCHING 8002c238 516
#include "TOBJ.H"
extern unsigned char DAT_800a6039;
extern unsigned char DAT_800a603d;
extern short DAT_800a60b6;
extern unsigned char DAT_800a60d4;
extern void *DAT_8001473c;
extern void *DAT_80014740;
extern unsigned short *DAT_800a605c[];
extern int DAT_800a60c4;
extern unsigned short DAT_800a6066;
extern Fix16 *DAT_800a6078[];
extern Fix16 *DAT_800a607c[];
extern unsigned short DAT_800a604e[];
extern unsigned char DAT_800a6047[];
extern unsigned char DAT_800a6047b[];
extern signed char DAT_80011eb4[];
extern unsigned char DAT_80011dad[];
extern unsigned char DAT_800a6101[];
extern void FUN_8001fe6c(TObj *);

void FUN_8002c238(TObj *o)
{
    int d;
    o->d8c = 0;
    o->visible = DAT_800a6039;
    if (DAT_800a603d == 2 && DAT_800a60b6 >= -0xc7 && DAT_800a60d4 != 0) {
        o->anim = DAT_8001473c;
        FUN_8001fe6c(o);
        if (DAT_800a60b6 > 0 && *DAT_800a605c[0] == 0x47)
            o->d8c = DAT_800a60c4;
    } else if (DAT_800a603d == 4 && DAT_800a60b6 >= -0xc7) {
        o->anim = DAT_8001473c;
        FUN_8001fe6c(o);
        if (DAT_800a60b6 > 0)
            o->d8c = DAT_800a60c4;
    } else {
        o->anim = DAT_80014740;
        FUN_8001fe6c(o);
        d = DAT_800a60c4;
        o->d8c = (unsigned char)((DAT_800a6066 & 1) ? d - 0x20 : d + 0x20);
    }
    o->h->p.whole = DAT_800a6078[0]->p.whole;
    o->y.p.whole = DAT_800a604e[0] + 4;
    o->d->p.whole = DAT_800a607c[0]->p.whole;
    o->b0f = DAT_800a6047[0] + 1;
    o->b0f = DAT_800a6047b[0] + DAT_80011dad[DAT_80011eb4[*DAT_800a605c[0]] * 4];
    if (DAT_800a6101[0] == 0)
        o->b04 = 3;
}
