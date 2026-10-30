// VERSION SLPS_01144 x18 80116098 372
// Older build of PAL x018 initWitchMizuno (8011A1B4): same table init, different tail.
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

typedef struct { unsigned char b[12]; } R12;
typedef struct { unsigned short h[6]; } H12;
typedef struct { short s[6]; } I12;
typedef struct { int pad[2]; void **anim; } P12;

extern R12 D_801194AC[];
extern R12 D_801194AD[];
extern R12 D_801194AE[];
extern R12 D_801194AF[];
extern H12 D_801194B0[];
extern I12 D_801194B2[];
extern P12 D_801194B4[];
extern unsigned char D_8009E995;
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);

void func_80116098_slps01144(TObj *o)
{
    o->box0 = D_801194AC[o->subtype].b[0];
    o->box1 = D_801194AD[o->subtype].b[0];
    o->box2 = D_801194AE[o->subtype].b[0];
    o->box3 = D_801194AF[o->subtype].b[0];
    o->w1e = D_801194B0[o->subtype].h[0];
    o->d3c = SPR_DATA[D_801194B2[o->subtype].s[0]];
    o->anim = D_801194B4[o->subtype].anim[(unsigned short)o->wb4];
    AnimLoadDuration(o);
    o->d8c = 0;
    o->b0d = 0;
    o->b0a = 0;
    o->b0f = 0;
    o->step = 0;
    o->category |= 0x80;
    o->b04++;
    if (D_8009E995 != 0xff) o->b04 = 3;
}
#endif
