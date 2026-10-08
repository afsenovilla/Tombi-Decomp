// FUNC 8010b050 872 X001
// MATCHING 8010b050 872
#include "TOBJ.H"
typedef struct { char p[8]; unsigned char b8; unsigned char b9; char q[0x20 - 0xa]; unsigned short w20; char r[0x2c - 0x22]; short w2c; short w2e; } G;
extern G *D_8009C330;
extern TObj *D_800A611C[];
extern short D_8009C944[], D_8009C946[];
extern char D_80010748[];
extern unsigned char D_801152E8[];
void FUN_800eee90(TObj *o);
void FUN_8010f328(TObj *o);
void PlayerSetAnimIfChanged(TObj *o, int n);
void ObjGravityStep(TObj *o);
void ObjAddVelY7E(TObj *o);
int AnimAdvance(TObj *o);
short TileCollideAt(TObj *, short, short);
int ObjCheckHeadCollision(TObj *o);
short ObjTileCollide(TObj *, int, int);
void AnimLoadDuration(TObj *o);
void func_8010B050(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b69 = 0;
        o->b9e = 0;
        o->b9c = 1;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        D_8009C330->b8 = 0;
        D_8009C330->w20 = 0;
        switch (D_800A611C[0]->type) {
        case 0x15:
        case 0x18:
        case 0x4e:
            break;
        default:
            FUN_800eee90(o);
            break;
        }
        *(unsigned char *)&o->wac = 0;
        FUN_8010f328(o);
        PlayerSetAnimIfChanged(o, 0x2e);
        o->state = 1;
    case 1:
        if (--D_8009C330->b9 != 0) {
            if (D_8009C330->b8 == 0) {
                D_8009C330->w20++;
                FUN_8010f328(o);
            }
        } else {
            D_8009C330->b8 = 1;
            if (D_8009C330->w20 >= 5) {
                D_8009C330->b8 = 0;
                o->state = 2;
            } else {
                D_8009C330->w20++;
                FUN_8010f328(o);
            }
        }
    case 2:
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 3;
        }
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 3;
        }
        o->b69 = 0;
        break;
    case 3:
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->h->raw += o->velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 == 1 || ObjTileCollide(o, 0, 0)) {
            D_8009C330->b8 = 0;
            o->ba5 = 0;
            o->b9c = 0;
            *(unsigned char *)&o->wac = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            D_8009C330->w20 = 0;
            D_8009C330->w2c = 0;
            D_8009C330->w2e = 0;
            o->anim = D_80010748;
            AnimLoadDuration(o);
            o->d8c = D_801152E8[o->wb0];
            o->state++;
        }
        break;
    case 4:
        break;
    }
}
