// VERSION PCPX_96084 tomba 80033BA8 448
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of updateImpactEffect (src/updateImpactEffect.c): draw constants, scratchpad word 0x1F800160, a two-argument sound call; the volatile store keeps the anim store before the scratchpad load.
#include "TOBJ.H"
extern void *D_80011AF8[];
extern unsigned short D_800F5006;
extern Fix16 *D_800F5018;
extern void *IMPACT_EFFECT_ANIM;
extern unsigned char D_800A2AE2[];
extern unsigned short D_800F4FEE[];
extern int DAT_1f8002d8[];
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern void ObjListPush_1F800230(TObj *);
extern int func_80018550(TObj *);
extern int func_800180D8_pcpx96084(TObj *);
extern void func_8001A87C_pcpx96084(TObj *);

extern unsigned short D_800DE0D8;
extern int D_1F800160;
extern void func_80022F5C_pcpx96084(int, int);
void func_80033BA8_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    int d;
    switch (s) {
    case 0:
        o->w1e = 0x15;
        if (o->subtype == 3) {
            if (D_800DE0D8) o->w1e = 0xc;
            else o->w1e = 0xd;
        }
        o->b0d = 0;
        *(signed char *)&o->b0f = -12;
        *(short *)&o->b0a = 0;
        o->animFrame = 0;
        *(void * volatile *)&o->anim = D_80011AF8[o->subtype];
        o->d3c = D_1F800160;
        o->b04++;
        AnimLoadDuration(o);
        if (o->subtype == 2) {
            func_80022F5C_pcpx96084(0, D_800A2AE2[o->subtype]);
        }
        break;
    case 1:
        if (o->subtype == 2) {
            o->visible = 1;
            ObjListPush_1F800230(o);
            if (D_800F5006 & 1) d = -14; else d = 14;
            o->h->p.whole = D_800F5018->p.whole + d;
            o->y.p.whole = D_800F4FEE[0];
            if (func_800180D8_pcpx96084(o)) {
                o->b04++;
            }
        } else if (func_80018550(o)) {
            if (func_800180D8_pcpx96084(o)) {
                o->b04++;
            }
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
