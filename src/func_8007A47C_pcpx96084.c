// VERSION PCPX_96084 tomba 8007A47C 1172
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Breakable pile (SLPM-80165 updateA00L2Type06): phase/timers at +0xB0..+0xB4, +0x0A a halfword, an item drop on phase 2.
#include "TOBJ.H"
extern void *D_800AEC24[][3];
extern unsigned short *D_800AECB4[];
extern short D_800AEC04[];
extern short D_800AEC0C[][3];
extern volatile int D_1F80015C;
extern void AnimLoadDuration(TObj *);
extern int func_80018550(TObj *);
extern void func_80059B44_pcpx96084(int, short *, int, int);
extern void AnimJump(TObj *, int);
extern int Rand(void);
extern void func_80031C60_pcpx96084(TObj *, short, short, short);
extern void func_80022F5C_pcpx96084(int, int);
extern void func_80018E74_pcpx96084(int);
extern void func_800180D8_pcpx96084(TObj *);
extern void freeObjectLayer7(TObj *);

#define U(x) (*(unsigned short *)&(x))
#define BOX(o) { b = D_800AECB4[(o)->subtype] + U((o)->wb0) * 4; \
    (o)->box0 = *b++; (o)->box2 = *b++; (o)->box1 = *b; (o)->box3 = b[1]; }

void func_8007A47C_pcpx96084(TObj *o)
{
    short sv[6];
    unsigned short *b;
    TObj *q;
    int s;

    switch (o->b04) {
    case 0:
        o->w1e = 10;
        o->b0d = 0;
        U(o->wb0) = 0;
        U(o->wb2) = 0;
        U(o->wb4) = 0;
        *(void * volatile *)&o->anim = D_800AEC24[o->subtype][U(o->wb0)];
        o->d3c = D_1F80015C;
        o->animFrame = 1;
        *(short *)&o->b0a = 0;
        *(signed char *)&o->b0f = -12;
        switch (o->subtype) {
        case 0:
        case 2:
        case 3:
            BOX(o);
            break;
        case 1:
            o->active = 2;
            o->box0 = 0;
            o->box1 = 0;
            o->box2 = 0;
            o->box3 = 0;
            break;
        }
        o->timer = 0;
        o->b04++;
        AnimLoadDuration(o);
        break;
    case 1:
        if (func_80018550(o) == 0)
            break;
        if (o->b68) {
            if (o->b68 == 1) {
                U(o->wb0)++;
                if (U(o->wb0) >= D_800AEC04[o->subtype])
                    *(unsigned short *)&o->b0a = 0xffff;
                q = (TObj *)o->d94;
                if (q != 0 && U(o->wb0) - U(q->wb0) >= 2)
                    q->b68 = 1;
                if (o->subtype == 0 && U(o->wb0) == 2) {
                    sv[1] = o->h->p.whole - 0x28;
                    sv[3] = o->y.p.whole - 0x18;
                    sv[5] = 0;
                    func_80059B44_pcpx96084(0, sv, -0x100, -0x100);
                }
                o->anim = D_800AEC24[o->subtype][U(o->wb0)];
                AnimJump(o, 0);
                BOX(o);
                U(o->wb2) = (Rand() & 1) + 3;
                U(o->wb4) = (Rand() & 7) + 5;
                o->timer = 0x1e;
            }
            o->b68 = 0;
        }
        if (U(o->wb4) != 0) {
            if (--U(o->wb2) == 0) {
                s = (Rand() & 0x1f) + D_800AEC0C[o->subtype][U(o->wb0)];
                func_80031C60_pcpx96084(o, o->a.p.whole - s, o->y.p.whole - (Rand() & 0x3f), o->b.p.whole);
                if ((U(o->wb4) & 3) == 0)
                    func_80022F5C_pcpx96084(2, 7);
                U(o->wb2) = (Rand() & 1) + 1;
                U(o->wb4)--;
            }
        }
        if (o->timer != 0) {
            if (--o->timer == 0) {
                if (U(o->wb0) >= D_800AEC04[o->subtype]) {
                    func_80018E74_pcpx96084(o->b6b);
                    o->b04++;
                } else {
                    o->active = 1;
                }
            }
        }
        func_800180D8_pcpx96084(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#endif
