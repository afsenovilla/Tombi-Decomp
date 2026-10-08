// FUNC 8011dd30 340 X000
// MATCHING 8011dd30 340
#include "TOBJ.H"
extern short DAT_8007a1f0[];
extern short DAT_8007a5f0[];
extern void FUN_8011d9d8(TObj *);
extern void FUN_8011d670(TObj *);
extern void FUN_8011d300(TObj *);

void FUN_8011dd30(TObj *o)
{
    int dy, dx;
    TObj *p;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = (signed char)o->ba5 * DAT_8007a1f0[o->animFrame] << 4;
    dx = (signed char)o->ba5 * DAT_8007a5f0[o->animFrame] << 4;
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
        FUN_8011d9d8(o);
        break;
    case 1:
        FUN_8011d670(o);
        break;
    case 2:
        FUN_8011d300(o);
        break;
    }
}
