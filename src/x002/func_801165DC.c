// FUNC 801165dc 480 X002
// MATCHING 801165dc 480
#include "TOBJ.H"

extern void *D_8011F260[];
extern void *D_8011F184;
extern int D_1F8002D4;
extern unsigned short GetClut(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void func_801165DC(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 1;
        o->animFrame = 1;
        o->b0a = 0;
        switch (o->subtype) {
        case 0:
            o->w1e = 4;
            o->w08 = GetClut(0x80, 0x1ff);
            o->anim = D_8011F260[o->subtype];
            *(signed char *)&o->b0f = -10;
            break;
        case 1:
            o->w1e = 4;
            o->w08 = GetClut(0x80, 0x1ff);
            o->anim = D_8011F260[o->subtype];
            o->b0f = 0;
            o->b0d = 0x81;
            break;
        case 2:
        case 3:
            o->w1e = 4;
            o->w08 = GetClut(0x80, 0x1ff);
            o->anim = D_8011F260[o->subtype];
            o->b0f = 0;
            break;
        case 4:
            o->w1e = 10;
            o->w08 = GetClut(0x80, 0x1fe);
            o->b0f = 0;
            o->anim = D_8011F184;
            break;
        }
        o->d3c = D_1F8002D4;
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->subtype != 4 && ObjCullRegister(o))
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
