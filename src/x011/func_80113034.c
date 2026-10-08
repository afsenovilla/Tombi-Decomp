// FUNC 80113034 900 X011
// MATCHING 80113034 900
#include "TOBJ.H"
extern unsigned char D_8009D0AA;
extern unsigned char D_8009D0C8;
extern unsigned char D_8009D0D0;
extern unsigned char D_8009D0D1;
extern int D_8009C960[];
extern unsigned short D_8009C960h;
extern void **D_80115A08[];
extern void **D_80115948[];
extern unsigned short DAT_1f8001f8;
extern void ObjCullRegister(TObj *);
extern void func_8004D620(int, int);
extern void FUN_80111b40(TObj *);
extern void AnimLoadDuration(TObj *);
extern void SfxPlay2(int, int);
extern void FUN_80020490(TObj *);
extern int Rand(void);
extern void FUN_800eb8f0(TObj *, int, int, int);

static __inline__ short chk(TObj *o)
{
    unsigned char f;
    switch (o->b0c & 0x7f) {
    case 0:
        f = D_8009D0AA;
        break;
    case 1:
        f = D_8009D0C8;
        break;
    case 2:
        f = D_8009D0D0;
        break;
    case 3:
        f = D_8009D0D1;
        break;
    default:
        func_8004D620(0, 2);
        return 0;
    }
    if (f)
        return 1;
    func_8004D620(0, 2);
    return 0;
}

void func_80113034(TObj *o)
{
    ObjCullRegister(o);
    switch (o->step) {
    case 0:
        break;
    case 1:
        if (chk(o)) {
            FUN_80111b40(o);
            o->step = 3;
            o->timer = 10;
            o->wac = 1;
            if (D_8009C960[0] == 0x30009)
                o->anim = D_80115A08[o->b0c & 0x7f][1];
            else
                o->anim = D_80115948[D_8009C960h * 4 + (o->b0c & 0x7f)][1];
            AnimLoadDuration(o);
            SfxPlay2(0x18, 8);
        } else {
            SfxPlay2(0x18, 0x12);
            o->step = 5;
            o->timer = 10;
        }
        break;
    case 2:
        o->step = 6;
        break;
    case 3:
        if (--o->timer == -1) {
            o->active = 3;
            o->step++;
        }
        break;
    case 4:
        if (o->visible)
            FUN_80020490(o);
        else
            o->b04 = 3;
        break;
    case 5:
        if (--o->timer == -1)
            o->step++;
        o->y.p.whole = o->d34 + (DAT_1f8001f8 & 1);
        o->h->p.whole = (short)(o->d30 - 2) + (Rand() & 3);
        break;
    case 6:
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        o->y.p.whole = o->d34;
        o->h->p.whole = o->d30;
        break;
    case 7:
        if (!chk(o)) {
            SfxPlay2(0x18, 0x12);
            o->step = 5;
            o->timer = 10;
        } else {
            FUN_800eb8f0(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->b04++;
        }
        break;
    }
}
