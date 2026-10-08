// FUNC 800f3f3c 1040 X000
// MATCHING 800f3f3c 1040
#include "TOBJ.H"
typedef struct {
    TObj t;
    unsigned char c0[3];
    unsigned char c3;
    unsigned char c4[2];
    unsigned char c6;
    unsigned char c7;
    unsigned char c8;
    unsigned char c9[0xe4 - 0xc9];
    int e4;
} P800F3F3C;

typedef struct {
    unsigned char b0;
    unsigned char p1[6];
    unsigned char b7;
    unsigned char b8;
    unsigned char p9[0x20 - 9];
    unsigned short w20;
    unsigned char p22[0x2c - 0x22];
    unsigned short w2c;
    unsigned short w2e;
} Q800F3F3C;

extern Q800F3F3C *D_8009C330;
extern unsigned short D_8009D670;
extern short D_8009C944;
extern short D_8009C946[];
extern int D_8009D2E8;
extern void FUN_8010f328(P800F3F3C *);
extern void SfxPlay2(int, int);
extern void func_8010F400(P800F3F3C *);
extern void func_8010EAF8(P800F3F3C *);
extern void ObjGravityStep(P800F3F3C *);
extern void ObjAddVelY7E(P800F3F3C *);
extern short TileCollideAt(P800F3F3C *, int, int);
extern int ObjCheckHeadCollision(P800F3F3C *);
extern void ObjSetAnimFromTable(P800F3F3C *);
extern void AnimJump(P800F3F3C *, int);

void func_800F3F3C(P800F3F3C *o)
{
    switch (o->t.state) {
    case 0:
        o->t.b9e = 0;
        o->t.b69 = 0;
        o->t.b9c = 1;
        *(unsigned char *)&o->t.da0 = 0;
        o->t.b9e = 0;
        *(unsigned char *)&o->t.waa = 0;
        o->c3 = 0;
        o->t.d8c = 0;
        o->t.wb0 = 0;
        o->t.wb6 = 0;
        o->t.velX = 0;
        D_8009C330->w20 = 0;
        D_8009C330->b8 = 0;
        FUN_8010f328(o);
        o->t.y.p.whole -= 4;
        SfxPlay2(2, 4);
        o->t.state = 1;
    case 1:
        if (o->t.velY >= 0) {
            D_8009C330->b8 = 1;
            o->t.state = 2;
        }
        if (*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C6) {
            if (D_8009C330->b8 != 0) goto common;
            if (++D_8009C330->w20 >= 0xe) {
                D_8009C330->b8 = 1;
                o->t.state = 2;
            }
        } else {
            D_8009C330->b8 = 1;
            if (D_8009C330->w20 >= 5) {
                o->t.state = 2;
                goto common;
            }
            D_8009C330->w20++;
        }
        FUN_8010f328(o);
    case 2:
    common:
        o->t.h->raw += D_8009C944 << 8;
        o->t.y.raw += D_8009C946[0] << 8;
        func_8010F400(o);
        func_8010EAF8(o);
        o->t.h->raw += o->t.velX << 8;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        if (*(unsigned char *)&o->t.wac == 2) {
            D_8009D2E8 = o->e4;
            D_8009C330->b8 = 0;
            o->t.ba5 = 0;
            o->t.step = 0xe;
            o->t.state = 0;
            break;
        }
        TileCollideAt(o, o->t.h->p.whole, (short)(o->t.y.p.whole + 16));
        if (ObjCheckHeadCollision(o)) {
            D_8009C330->b8 = 1;
            D_8009C330->b7 = o->t.animFrame;
            o->t.b9c = 2;
            o->t.timer = 10;
            o->t.step = 4;
            o->t.velY = 0;
            *(unsigned char *)&o->t.wac = 1;
            o->t.state = 3;
            o->t.velY = 0;
            o->t.velV = 0;
        }
        if (o->t.velY > 0) {
            D_8009C330->b8 = 1;
            D_8009C330->b7 = o->t.animFrame;
            o->t.b9c = 2;
            o->t.timer = 10;
            o->t.step = 4;
            o->t.velY = 0;
            *(unsigned char *)&o->t.wac = 1;
            o->t.state = 3;
        } else if (D_8009C330->b0 == 0 && o->c6 == 0) {
            o->t.animFrame = (signed char)D_8009C330->b7;
            D_8009C330->w2c = 4;
            if (D_8009C330->w2e != 4) {
                D_8009C330->w2c = 4;
                ObjSetAnimFromTable(o);
                AnimJump(o, 0);
                D_8009C330->w2e = D_8009C330->w2c;
            }
            o->t.b9d = 0;
            o->t.timer = 0;
            o->t.d84 = 0;
            o->t.d88 = (o->t.animFrame & 1) ? 0xf0 : 0x10;
            o->t.d8c = 0;
            o->t.step = 2;
        }
        if (o->c8) {
            D_8009C330->b8 = 0;
            *(unsigned char *)&o->t.wac = 0;
            o->t.ba7 = 0;
            o->t.b9c = 0;
            o->t.step = 0x32;
            o->t.state = 0;
        }
        break;
    }
}
