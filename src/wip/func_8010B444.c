// FUNC 8010b444 832 X000
/* r9 score 6: only the last block: game hoists 'la D_80010748' between the G+0x2e and G+0x20 stores (G in v1) and stores o->anim in the jal delay slot. */
#include "TOBJ.H"

#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
#define G D_8009C330
extern unsigned char *D_8009C330;
extern short D_8009C944[], D_8009C946[];
extern char D_80010748[];
extern unsigned char D_801152E8[];
void FUN_8010f328(TObj *o);
void PlayerSetAnimIfChanged(TObj *o, int a);
void SfxPlay2(int a, int b);
void func_800EEC40(TObj *o);
void AnimAdvance(TObj *o);
void ObjGravityStep(TObj *o);
void ObjAddVelY7E(TObj *o);
void TileCollideAt(TObj *o, short x, short y);
int ObjCheckHeadCollision(TObj *o);
short ObjTileCollide(TObj *o, int a, int b);
void AnimLoadDuration(TObj *o);

void func_8010B444(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b69 = 0;
        B(o, 0x9e) = 0;
        o->b9c = 1;
        o->wb2 = 0;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        G[8] = 0;
        *(short *)(G + 0x20) = 0;
        B(o, 0xac) = 0;
        FUN_8010f328(o);
        PlayerSetAnimIfChanged(o, 4);
        SfxPlay2(2, 4);
        o->state = 1;
    case 1:
        if (--G[9] != 0) {
            if (G[8] == 0) {
                (*(unsigned short *)(G + 0x20))++;
                FUN_8010f328(o);
            }
        } else {
            G[8] = 1;
            if (*(unsigned short *)(G + 0x20) >= 5) {
                G[8] = 0;
                o->state = 2;
            } else {
                (*(unsigned short *)(G + 0x20))++;
                FUN_8010f328(o);
            }
        }
    case 2:
        func_800EEC40(o);
        AnimAdvance(o);
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 3;
        }
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            o->velY = 0;
            *(short *)((char *)o + 0x82) = 0;
            o->state = 3;
        }
        o->b69 = 0;
        break;
    case 3:
        func_800EEC40(o);
        AnimAdvance(o);
        B(o, 0x9e) = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        if (o->b69 == 1 || ObjTileCollide(o, 0, 0)) {
            G[8] = 0;
            B(o, 0xa5) = 0;
            o->b9c = 0;
            B(o, 0xac) = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            *(unsigned short *)(G + 0x2e) = 0xffff;
            o->anim = D_80010748;
            *(short *)(G + 0x20) = 0;
            *(short *)(G + 0x2c) = 0;
            AnimLoadDuration(o);
            o->d8c = D_801152E8[o->wb0];
            o->state = 4;
        }
        break;
    case 4:
        break;
    }
}
