// FUNC 8011ad98 828 X000
// MATCHING 8011ad98 828
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);
extern unsigned short GetClut(int, int);
extern void FUN_8011ac58(TObj *);
extern void *D_8013B2A4;
extern void *D_8013B2A8;
extern void *D_8013B2AC[];
extern void *D_8013B2B8;
extern void *D_8013B2BC;
extern void *D_8013B2C0;
extern void *D_8013B2C4a[]; /* array: keeps its load after the field store (case 6 schedules differently from case 5) */
extern void *D_8013B2C8;

void func_8011AD98(TObj *o)
{
    int x;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 0x80;
        o->animFrame = 1;
        o->w1e = 0xb;
        o->b0a = 7;
        o->b.p.whole = 200;
        o->b0f = 0;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 0:
            o->anim = D_8013B2A4;
            break;
        case 1:
            o->b0d = 1;
            o->anim = D_8013B2A8;
            o->timer = 10;
            o->w22 = 0;
            break;
        case 2:
            o->anim = D_8013B2AC[o->b0c];
            break;
        case 3:
            o->b0d = 0x81;
            o->anim = D_8013B2B8;
            o->timer = 10;
            o->w22 = 0;
            break;
        case 4:
            o->anim = D_8013B2BC;
            o->w1e = 9;
            o->timer = 10;
            o->b0d = 0;
            o->w22 = 0;
            o->animFrame = 1;
            o->state = 0;
            break;
        case 5:
            o->b.p.whole = 0;
            o->anim = D_8013B2C0;
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->animFrame = 1;
            o->w1e = 7;
            o->b0f = 10;
            break;
        case 6:
            o->b.p.whole = 0;
            o->anim = D_8013B2C4a[0];
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->animFrame = 1;
            o->w1e = 7;
            o->b0f = 10;
            break;
        case 7:
            o->anim = D_8013B2C8;
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->animFrame = 1;
            o->w1e = 7;
            break;
        }
        o->d3c = ((struct { char pad[0x2d4]; int v; } *)0x1F800000)->v;
        AnimLoadDuration(o);
        break;
    case 1:
        o->visible = 1;
        o->a.p.whole = o->d30 - ((struct { char pad[0x176]; unsigned short v; } *)0x1F800000)->v;
        o->y.p.whole = o->d34 - ((struct { char pad[0x186]; unsigned short v; } *)0x1F800000)->v;
        ObjListPush_1F800220(o);
        switch (o->subtype) {
        case 1:
            if (--o->timer == 0) {
                x = 0xd0;
                goto common;
            }
            break;
        case 3:
            if (--o->timer == 0) {
                x = 0xe0;
            common:
                o->timer = 10;
                o->w22 ^= 1;
                o->w08 = GetClut(x, o->w22 + 0x1ed);
            }
            break;
        case 4:
            FUN_8011ac58(o);
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
