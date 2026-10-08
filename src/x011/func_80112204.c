// FUNC 80112204 796 X011
// MATCHING 80112204 796
#include "TOBJ.H"
extern short *D_800A6078;
extern unsigned short D_800A604E[];
extern char D_80115AFA[];
extern int D_8009C960[];
extern unsigned short D_8009C960h;
extern int *D_80115A08[];
extern int *D_80115948[];
extern int ObjCullRegister(TObj *o);
extern void FUN_80020c04(int n);
extern void FUN_8003ecb0(char *a, Fix16 *v, int b, int c, TObj *o);
extern void AnimLoadDuration(TObj *o);
extern void FUN_80020490(TObj *o);
extern void func_80111DC8(TObj *o);
extern void FUN_800eb8f0(TObj *o, int x, int y, int z);

void func_80112204(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    short dx, dy;
    Fix16 v[3];
    dx = p->h->p.whole - o->wb4;
    dy = p->y.p.whole - o->wb6;
    o->h->p.whole += dx;
    o->y.p.whole += dy;
    o->wb4 = p->h->p.whole;
    o->wb6 = p->y.p.whole;
    ObjCullRegister(o);
    switch (o->step) {
    case 0:
        D_800A6078[1] += dx;
        D_800A604E[0] += dy;
        break;
    case 1:
        FUN_80020c04(o->b6b);
        v[0].p.whole = o->h->p.whole;
        v[1].p.whole = o->y.p.whole;
        v[2].p.whole = o->d->p.whole;
        FUN_8003ecb0(D_80115AFA, v, -0x40, -0x400, o);
        FUN_8003ecb0(D_80115AFA, v, 0x40, -0x400, o);
        o->step = 3;
        o->timer = 10;
        o->wac = 1;
        if (D_8009C960[0] == 0x30009)
            o->anim = (void *)D_80115A08[o->b0c & 0x7f][1];
        else
            o->anim = (void *)D_80115948[D_8009C960h * 4 + (o->b0c & 0x7f)][1];
        AnimLoadDuration(o);
        o->b69 = 0;
        o->velX = 0x80;
        o->velY = 0x100;
        o->w78 = ((o->d38 + 0x800) & 0xfff) >> 4;
        if (o->w78 > 0x80) o->w78 = 0x80;
        o->d8c = o->w78 + 0x80;
        break;
    case 2:
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        break;
    case 3:
        if (--o->timer == -1) {
            o->active = 3;
            o->step++;
        }
    case 4:
        if (o->visible == 0) {
            o->b04 = 3;
            break;
        }
        FUN_80020490(o);
        func_80111DC8(o);
        if (o->y.p.whole < -0xae) break;
        o->timer = 10;
        o->active = 2;
        o->step = 7;
        break;
    case 5:
        FUN_800eb8f0(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        FUN_80020c04(o->b6b);
        o->b04++;
        break;
    case 7:
        FUN_800eb8f0(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->b04++;
        break;
    }
}
