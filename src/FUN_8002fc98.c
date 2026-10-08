// FUNC 8002fc98 480 MAIN0
// MATCHING 8002fc98 480
#include "TOBJ.H"
extern void ObjCullRegister(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void FUN_8002f16c(TObj *);
extern void FUN_8002f4e0(TObj *);
extern void FUN_8002f960(TObj *);
extern void FUN_8002f384(TObj *);
extern int *DAT_80079d58[];
extern unsigned char DAT_800a6039;
extern unsigned char DAT_800a6047;
extern unsigned char DAT_8009c990;
extern unsigned short DAT_1f8001f8;

void FUN_8002fc98(TObj *o)
{
    char c;
    ObjCullRegister(o);
    switch (o->step) {
    case 0:
        switch (o->subtype) {
        case 0:
            FUN_8002f16c(o);
            break;
        case 1:
            FUN_8002f4e0(o);
            break;
        case 2:
            FUN_8002f960(o);
            break;
        case 9:
            FUN_8002f384(o);
            break;
        }
        if (o->subtype != 9 && (DAT_1f8001f8 & 3) == 0)
            SfxPlay2(3, 12);
        break;
    case 1:
        switch (o->state) {
        case 0:
            o->anim = (void *)DAT_80079d58[o->subtype * 3][o->b0c];
            AnimLoadDuration(o);
            o->visible = DAT_800a6039;
            c = DAT_800a6047;
            o->b6b = 0x7f;
            o->b0f = c - 1;
            if (DAT_8009c990 == 3)
                o->b6b = 0x40;
            o->state = o->state + 1;
            break;
        case 1:
            if (AnimAdvance(o)) {
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            }
            if (o->visible == 0) {
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        break;
    }
}
