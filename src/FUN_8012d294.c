// FUNC 8012d294 1384 X000
// MATCHING 8012d294 1384
#include "TOBJ.H"

extern int D_1F8002D0[];
extern int D_1F800190;
extern int D_1F80018C;
extern unsigned short D_1F800176;
extern short D_1F80027E;
extern unsigned char D_8009C93F;
extern unsigned char D_800A4553;
extern char D_80077D24[];
extern char D_80077CDC[];
extern char D_80077CF4[];
extern void *D_8013ABB8[];
extern void *D_8013ABBC[];
extern void *D_8013ABC0[];
extern short TileCollideAt(TObj *, short, short);
extern void ObjCullRegister(TObj *);
extern void AnimLoadDuration(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void applyFrameVelocityXY(TObj *);
extern void applyFrameVelocityY(TObj *);
extern void setEventStarted(int, int, int);
extern void addItemToInventory(int, int, int);
extern void FUN_80020aec(int);
extern void freeObjectLayer2(TObj *);

void FUN_8012d294(TObj *o)
{
    short d;

    switch (o->b04) {
    case 0:
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x10;
        o->d3c = D_1F8002D0[0];
        o->w1e = 1;
        o->b0d = 0;
        o->b0a = 0;
        o->b69 = 0;
        o->b68 = 0;
        o->b0f = 2;
        o->d8c = 0;
        o->wac = 0;
        o->b04++;
        o->anim = D_8013ABB8[0];
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->step++;
            o->timer = 0x1e;
            o->velV = -0x400;
            break;
        case 1:
            D_1F80018C = o->h->raw;
            D_1F800190 = o->y.raw;
            if (o->timer == 0) {
                o->y.raw += o->velV << 8;
                o->velV += 0x20;
                if (o->velV > 0x400)
                    o->velV = 0x400;
                if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8)) {
                    o->state = 3;
                    o->movetab = D_80077D24;
                    o->timer = 0x1e;
                    o->step++;
                    D_800A4553 = 6;
                }
            } else {
                o->timer--;
            }
            break;
        case 2:
            if (D_8009C93F)
                break;
            switch (o->state) {
            case 0:
                switch (Rand() & 3) {
                case 0:
                    o->state = 2;
                    o->movetab = D_80077D24;
                    o->timer = 0x78;
                    o->wac = 1;
                    o->anim = D_8013ABBC[0];
                    AnimLoadDuration(o);
                    ObjSetFacingToPlayer(o);
                    break;
                case 1:
                    o->state = 1;
                    o->movetab = D_80077CDC;
                    o->timer = 100;
                    o->wac = 1;
                    o->anim = D_8013ABBC[0];
                    AnimLoadDuration(o);
                    break;
                case 2:
                    o->state = 2;
                    o->movetab = D_80077CF4;
                    o->timer = 0x32;
                    o->wac = 1;
                    o->anim = D_8013ABBC[0];
                    AnimLoadDuration(o);
                    o->animFrame = Rand() & 1;
                    break;
                case 3:
                    o->state = 3;
                    o->movetab = D_80077D24;
                    o->timer = 0x96;
                    o->wac = 2;
                    o->anim = D_8013ABC0[0];
                    AnimLoadDuration(o);
                    break;
                }
                return;
            case 2:
                AnimAdvance(o);
                if (o->timer == 1) {
                    o->state = 1;
                    o->movetab = D_80077CDC;
                    o->timer = 0x28;
                }
            case 1:
                AnimAdvance(o);
                applyFrameVelocityXY(o);
                d = o->a.p.whole - 0x53;
                if ((unsigned short)d >= 0x40e) {
                    if (d < 0)
                        o->animFrame = 0;
                    else
                        o->animFrame = 1;
                } else {
                    d = o->a.p.whole - D_1F800176;
                    if ((unsigned short)d >= 0x141) {
                        if (d < 0)
                            o->animFrame = 0;
                        else
                            o->animFrame = 1;
                    }
                }
                break;
            case 3:
                AnimAdvance(o);
                applyFrameVelocityY(o);
                break;
            default:
                return;
            }
            if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8))
                o->d8c = (-D_1F80027E << 2) & 0xff;
            if (--o->timer == -1)
                o->state = 0;
            break;
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->step++;
            o->wac = 0;
            o->anim = D_8013ABB8[0];
            AnimLoadDuration(o);
            break;
        case 1:
            AnimAdvance(o);
            break;
        case 2:
            setEventStarted(0xa7, 0, 1);
            addItemToInventory(0x23, 1, 1);
            FUN_80020aec(o->b6b);
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
