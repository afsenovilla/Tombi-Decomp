// FUNC 8002fc98 480 MAIN0
// MATCHING 8002fc98 480
#include "TOBJ.H"
extern void ObjCullRegister(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void playerEffect4EStepA(TObj *);
extern void playerEffect4EStepC(TObj *);
extern void playerEffect4EStepD(TObj *);
extern void playerEffect4EStepB(TObj *);
extern int *PLAYER_EFFECT_4E_ANIM_TABLE[];
extern unsigned char DAT_800a6039;
extern unsigned char DAT_800a6047;
extern unsigned char DAT_8009c990;
extern unsigned short DAT_1f8001f8;

void playerEffect4EDispatchB(TObj *o)
{
    char c;
    ObjCullRegister(o);
    switch (o->step) {
    case 0:
        switch (o->subtype) {
        case 0:
            playerEffect4EStepA(o);
            break;
        case 1:
            playerEffect4EStepC(o);
            break;
        case 2:
            playerEffect4EStepD(o);
            break;
        case 9:
            playerEffect4EStepB(o);
            break;
        }
        if (o->subtype != 9 && (DAT_1f8001f8 & 3) == 0)
            SfxPlay2(3, 12);
        break;
    case 1:
        switch (o->state) {
        case 0:
            o->anim = (void *)PLAYER_EFFECT_4E_ANIM_TABLE[o->subtype * 3][o->b0c];
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
