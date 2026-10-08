// FUNC 80108338 976 X003
// MATCHING 80108338 976
#include "TOBJ.H"

extern unsigned short D_8009C960[];
extern unsigned short D_8009C962;
extern short D_8009C944[];
extern short D_8009C946[];
extern char *D_8009C330;
extern unsigned char D_8009C93A;
extern unsigned char D_8009D2B0;
void func_800EEC40(TObj *o);
void AnimAdvance(TObj *o);
void PlayerSetAnimIfChanged(TObj *o, int a);
void SfxPlay2(int a, int b);
short ObjTileCollide(TObj *o, int a, int b);
void TileCollideAt(TObj *o, int x, int y);
int ObjCheckHeadCollision(TObj *o);

void func_80108338(TObj *o)
{
    switch (o->state) {
    case 0:
        o->active = 4;
        o->velH = 0;
        o->velV = 0;
        if ((D_8009C960[0] == 2 || D_8009C960[0] == 0x13) && D_8009C962 == 1) {
            short v = 0x100;
            if (o->animFrame & 1)
                v = -0x100;
            o->velX = v;
            o->velY = -0xa00;
        } else {
            if (D_8009C960[0] != 4) {
                o->active = 1;
                o->velX = 0;
                o->velY = 0;
                D_8009C330[8] = 0;
                o->ba5 = 0;
                o->b9c = 0;
                *(unsigned char *)&o->wac = 0;
                *(signed char *)&o->b0f = -8;
                o->wb2 = 0;
                *(short *)(D_8009C330 + 0x20) = 0;
                o->step = 2;
                o->b04 = 1;
                o->state = 3;
                o->substep = 0;
                o->timer = 0;
                D_8009D2B0 = 0;
                D_8009C93A = 1;
                return;
            }
            o->velX = 0;
            o->velY = 0;
        }
        o->b69 = 0;
        o->b9e = 0;
        o->b9c = 1;
        o->wb2 = 0;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        D_8009C330[8] = 0;
        *(short *)(D_8009C330 + 0x20) = 0;
        *(unsigned char *)&o->wac = 0;
        PlayerSetAnimIfChanged(o, 4);
        SfxPlay2(2, 4);
        o->state = 1;
    case 1:
        func_800EEC40(o);
        AnimAdvance(o);
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0) {
            o->active = 3;
            o->b9c = 2;
            o->state++;
        }
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        if (ObjCheckHeadCollision(o)) {
            o->active = 3;
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 2;
        }
        o->b69 = 0;
        break;
    case 2:
        func_800EEC40(o);
        AnimAdvance(o);
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0x680)
            o->velY = 0x680;
        if (o->b69 == 0 && ObjTileCollide(o, 0, 0) == 0)
            return;
        o->active = 1;
        D_8009C330[8] = 0;
        o->ba5 = 0;
        o->b9c = 0;
        *(unsigned char *)&o->wac = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        *(short *)(D_8009C330 + 0x20) = 0;
        *(signed char *)&o->b0f = -8;
        *(short *)(D_8009C330 + 0x20) = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        o->timer = 0;
        D_8009C93A = 1;
        break;
    }
}
