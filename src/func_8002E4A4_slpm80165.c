// VERSION SLPM_80165 slpm80165 8002E4A4 468
// FLAGS.SLPM_80165 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version: SLPS_01144 C of src/func_8002F7C0_slps01144.c (func_8002F7C0, twin score 0.960) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#include "TOBJ.H"
extern short COS_TABLE[];
extern short NEG_SIN_TABLE[];
extern unsigned short TICK_COUNT;
extern int UPDATE_OBJECT_INDEX;
extern TObj *ObjAlloc(void);
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

extern void func_8002FD60_slps01144(TObj *);
static __inline__ void setup(TObj *p)
{
    p->b04 = 1;
    func_8002FD60_slps01144(p);
}

void func_8002F7C0(TObj *o)
{
    TObj *p;
    int r;
    int s;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if ((TICK_COUNT + UPDATE_OBJECT_INDEX) & 1) {
                if (--o->timer == -1) {
                    o->b04 = 3;
                    break;
                }
                p = ObjAlloc();
                if (p != 0) {
                    r = Rand();
                    s = ((r & 1) << 11) + 0x800;
                    p->h->raw = o->h->raw;
                    p->y.raw = o->y.raw;
                    p->d->raw = o->d->raw;
                    p->h->raw += (COS_TABLE[r & 0xf0] * s) >> 4;
                    p->y.raw += (NEG_SIN_TABLE[r & 0xf0] * s) >> 4;
                    setup(p);
                }
            }
            break;
        case 1:
            setup(o);
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (AnimAdvance(o) != 0) o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
