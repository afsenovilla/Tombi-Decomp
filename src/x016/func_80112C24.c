// FUNC 80112c24 1040 X016
// MATCHING 80112c24 1040
/* D_8009C960h (halfword name for D_8009C960) stays: a cast, a union or -fno-expensive-optimizations all CSE the la (21). */
#include "TOBJ.H"
extern unsigned char D_8009D0AA, D_8009D0C8, D_8009D0D0, D_8009D0D1;
extern int D_8009C960[];
extern unsigned short D_8009C960h[];
extern unsigned short D_1F8001F8;
extern int *D_80115A08[];
extern int *D_80115948[];
void ObjCullRegister(TObj *o);
void func_8004D620(int a, int b);
void FUN_80111b40(TObj *o);
void AnimLoadDuration(TObj *o);
void SfxPlay2(int a, int b);
void FUN_8005a9a4(int a, int b);
short TileCollideAt(TObj *o, int x, int y);
void FUN_800eb8f0(TObj *o, int a, int b, int c);
void FUN_80020490(TObj *o);
int Rand(void);

static __inline__ int check(TObj *o)
{
    unsigned char v;
    switch (o->b0c & 0x7f) {
    case 0: v = D_8009D0AA; break;
    case 1: v = D_8009D0C8; break;
    case 2: v = D_8009D0D0; break;
    case 3: v = D_8009D0D1; break;
    default: goto fail;
    }
    if (v) return 1;
fail:
    func_8004D620(0, 2);
    return 0;
}

void func_80112C24(TObj *o)
{
    ObjCullRegister(o);
    switch (o->step) {
    case 0:
        break;
    case 1:
        if (check(o)) {
            FUN_80111b40(o);
            o->step = 3;
            o->timer = 10;
            o->wac = 1;
            if (D_8009C960[0] == 0x30009) {
                o->anim = (void *)D_80115A08[o->b0c & 0x7f][1];
            } else {
                o->anim = (void *)D_80115948[D_8009C960h[0] * 4 + (o->b0c & 0x7f)][1];
            }
            AnimLoadDuration(o);
            SfxPlay2(0x18, 8);
            if (o->subtype == 6) {
                FUN_8005a9a4(0x19, 0);
            }
            break;
        }
        goto fail;
    case 2:
        o->step = 6;
        break;
    case 3:
        if (--o->timer == -1) {
            o->step++;
        }
        break;
    case 4:
        o->velV += 0x20;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        o->y.raw += o->velV << 8;
        if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
            FUN_800eb8f0(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->b04 = 3;
        } else {
            FUN_80020490(o);
        }
        break;
    case 5:
        if (--o->timer == -1) {
            o->step++;
        }
        o->y.p.whole = o->d34 + (D_1F8001F8 & 1);
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
        if (!check(o)) {
        fail:
            SfxPlay2(0x18, 0x12);
            o->step = 5;
            o->timer = 0x14;
            break;
        }
        FUN_800eb8f0(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->b04++;
        break;
    }
}
