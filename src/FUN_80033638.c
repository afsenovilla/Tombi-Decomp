// FUNC 80033638 392 MAIN0
// MATCHING 80033638 392
#include "TOBJ.H"
extern short MulCos(int, int);
extern short MulNegSinScaled(int, int);
extern void ObjApplyVelocity(TObj *);
extern short FUN_800411cc(TObj *, int, int);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_800eaffc(int, int, int, int);
extern void SfxPlay(int);
extern void FUN_800efa80(TObj *);
extern unsigned short DAT_1f800282;
extern int DAT_800a60c4[];
extern unsigned char DAT_800a60d5[];
extern unsigned char *DAT_8009c330;
extern short DAT_8007a074[];

static __inline__ void f(TObj *q)
{
    q->waa = DAT_8007a074[(short)(q->animFrame + q->animFrame * 2)];
    DAT_800a60d5[0] = 0;
    *DAT_8009c330 = 0;
    FUN_800efa80(q);
}

int FUN_80033638(TObj *o)
{
    int r;
    unsigned char *p;
    short t;
    unsigned short u;
    int m;
    r = 0;
    o->d84 = 0x200;
    t = o->wa8 - 0x200;
    o->d88 = o->d88 + 0x100 & 0xfff;
    o->wa8 = t;
    o->velH = MulCos((unsigned char)o->waa, t);
    o->velV = MulNegSinScaled((unsigned char)o->waa, o->wa8);
    ObjApplyVelocity(o);
    if (o->active == 1 && FUN_800411cc(o, o->h->p.whole, o->y.p.whole) != 0) {
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        u = DAT_1f800282;
        m = u >> 5 & 0xf;
        if ((u & 0x3000) == 0 && m < 4 && m != 0)
            FUN_800eaffc(o->h->p.whole, o->y.p.whole, o->d->p.whole, (unsigned char)o->animFrame);
        SfxPlay(5);
        o->wa8 = 0x4ff;
    }
    DAT_800a60c4[0] = 0;
    if (o->wa8 < 0x500) {
        f(o);
        r = 1;
    }
    return r;
}
