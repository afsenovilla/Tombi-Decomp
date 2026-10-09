// FUNC 8011de64 512 X009
// MATCHING 8011de64 512
#include "TOBJ.H"
typedef struct {
    char p0[2]; short w2; char p4[4]; unsigned char b8, b9; char pa[2]; short wc, we;
    char p10[0x28 - 0x10]; unsigned short w28, w2a;
} G;
extern G *D_8009C330;
extern TObj *D_8009F0EC;
extern unsigned short D_1F8001FC, D_1F8003C6;
extern int AnimAdvance(TObj *o);
extern void FUN_800eee90(TObj *o);
extern void SfxPlay3(int, int);
extern void PlayerSetAnimIfChanged(TObj *o, int);
extern void FUN_800ef1ac(TObj *o);

void func_8011DE64(TObj *o)
{
    G *g;

    o->h->p.whole += D_8009F0EC->velX;
    o->y.p.whole += D_8009F0EC->velY;
    switch (o->state) {
    case 0:
        g = D_8009C330;
        o->wb6 = 0;
        g->b8 = 0;
        g->we = 0;
        g->w2 = 0;
        D_8009C330->b9 = 0;
        D_8009C330->wc = 0;
        D_8009C330->w28 = 0xffff;
        D_8009C330->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->h->p.whole = o->h->p.whole;
        o->y.p.whole += 4;
        FUN_800eee90(o);
        SfxPlay3(4, 0x7f);
        PlayerSetAnimIfChanged(o, 7);
        o->state++;
    case 1:
        AnimAdvance(o);
        if (D_1F8001FC & D_1F8003C6) {
            D_8009C330->b9 = 0;
            o->b04 = 1;
            o->step = 0x27;
            o->state = 0;
            o->y.p.whole += 0x10;
        }
        if (D_1F8001FC & 0x10) {
            D_8009C330->b9 = 0;
            o->b04 = 1;
            o->step = 0x27;
            o->state = 0;
            o->y.p.whole += 0x10;
        }
        if (D_1F8001FC & 0x40) {
            D_8009C330->b9 = 0;
            o->b9e = 6;
            o->step = 0x25;
            o->state = 0;
            o->y.p.whole += 9;
        }
        break;
    }
    FUN_800ef1ac(o);
}
