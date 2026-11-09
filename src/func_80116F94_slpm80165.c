// VERSION SLPM_80165 x00 80116F94 220
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 8011DD30 (updateChainedPartsObj.c): BETA_980203 C of src/updateChainedPartsObj.c (updateChainedPartsObj, twin score 0.554), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
extern short DAT_8007a1f0[];
extern short COS_TABLE[];
extern void FUN_8011d9d8(TObj *);
extern void FUN_8011d670(TObj *);
extern void FUN_8011d300(TObj *);

void updateChainedPartsObj(TObj *o)
{
    int dy, dx;
    TObj *p;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = (signed char)o->ba5 * DAT_8007a1f0[o->animFrame] << 4;
    dx = (signed char)o->ba5 * COS_TABLE[o->animFrame] << 4;
    p = (TObj *)o->d94;
    p->y.raw = o->y.raw + dy + (o->velY << 16);
    o->d34 = p->y.p.whole;
    p->a.raw = o->a.raw + dx;
    o->d30 = p->a.p.whole;
    o->box1 = (dx < 0 ? -dx >> 16 : dx >> 16);
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + ((dy < 0 ? -dy >> 16 : dy >> 16));
    switch (o->subtype) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    }
}
#endif
