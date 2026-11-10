// VERSION PCPX_96084 tomba 8007D618 300
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Breakable crate steps: hit (impact effect, area-specific animation D_8001410C[area * 7 + 1]), break pieces,
// a 10-frame wait before going inactive (3), then off-screen despawn.
#include "TOBJ.H"
extern void func_80018550(TObj *);
extern void func_8005A280_pcpx96084(int, void *, int, int);
extern void AnimLoadDuration(TObj *);
extern void func_80036BB0_pcpx96084(TObj *, short, short, short);
extern void func_80018704_pcpx96084(TObj *);
extern volatile unsigned short D_800DE0D8;
extern void *D_8001410C[];

void func_8007D618_pcpx96084(TObj *o)
{
    func_80018550(o);
    switch (o->step) {
    case 0:
        break;
    case 1:
        o->d->p.whole = 0x5a;
        func_8005A280_pcpx96084(0, &o->a, 0, -1280);
        o->step = 3;
        *(volatile short *)&o->timer = 10;
        o->wa8 = D_800DE0D8 * 7 + 1;
        o->anim = D_8001410C[o->wa8];
        AnimLoadDuration(o);
        break;
    case 2:
        func_80036BB0_pcpx96084(o, *(short *)((char *)o + 0x12), *(short *)((char *)o + 0x16), *(short *)((char *)o + 0x1a));
        o->b04++;
        break;
    case 3:
        if (--o->timer == -1) {
            o->active = 3;
            o->step++;
        }
        break;
    case 4:
        func_80018704_pcpx96084(o);
        break;
    }
}
#endif
