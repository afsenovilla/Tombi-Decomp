// FUNC 8013af10 484 X001
// MATCHING 8013af10 484
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned short D_800A6066;
extern unsigned short D_800A6066x[]; /* second name for D_800A6066: forces the reloads after the store */
extern TObj *D_800A60C8[];
extern Fix16 *D_800A6078;
extern unsigned char D_8009C93F, D_8009C93E, D_8009C942, D_8009C975, D_8009CE59, D_8009CEDE, D_8009CEDF;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned short D_8009C960, D_8009C962, D_8009CD94, D_8009CD96, D_8009CDA0;
extern char D_80010748[];
typedef struct { char p[0x4c]; short w4c, w4e; } SC;
extern SC *D_1F8001D4;
extern void AnimLoadDuration(TObj *);
extern TObj *FUN_8002dc50(int, int, int, int);

void func_8013AF10(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;

    switch (o->step) {
    case 0:
        D_8009C93F = 1;
        D_8009C93E = 1;
        D_8009C942 = 1;
        D_800A6038.anim = D_80010748;
        AnimLoadDuration(&D_800A6038);
        D_800A6066 = 0;
        e->b04 = 1;
        e->step = 0;
        e->state = 0;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_800A60C8[0] = FUN_8002dc50(2, 0x1e, 0x80, 0x6c);
        D_800A6066x[0] = D_800A6078->p.whole > e->h->p.whole;
        e->animFrame = D_800A6078->p.whole < e->h->p.whole;
        o->step++;
    case 1:
        p = D_800A60C8[0];
        if (p->b04 == 2) {
            p->b04 = 3;
            D_8009C975 = 3;
            o->step++;
        }
        break;
    case 2:
        if (D_8009C975 == 1) {
            D_8009CE59 = 1;
            D_8009CD94 = D_8009C960;
            D_8009CEDE = 0;
            D_8009CEDF = 0;
            D_8009CDA0 = 0;
            D_8009CD96 = D_8009C962;
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        break;
    }
}
