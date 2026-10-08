// FUNC 80133474 3152 X000
/* wip: only the ang sign-extension differs (game: andi at join then sll/sra 16 into s0).
 * AnimAdvance_ is a second extern name for AnimAdvance to stop cross-jumping state 1 into state 6 (debt).
 * NB: ncheck/matchcheck mask j targets; a version with the odd branch unmasked 'matches' but jumps past the andi. */
#include "TOBJ.H"
typedef struct { signed char anim, z, ang, rad; } E4;
typedef struct { char c[12]; } V12;
extern TObj D_800A6038;
extern unsigned char D_800A6039, D_800A6047, D_800A603C, D_800A603D, D_800A60DA;
extern unsigned short D_800A6066;
extern short D_800A604E;
extern unsigned short *D_800A605C[];
extern Fix16 *D_800A6078, *D_800A607C;
extern int D_800A60C4, D_800A609C;
extern Fix16 *D_800A607Ca[];
extern int D_800A60C4a[];
extern unsigned char D_800A6047a[];
extern int D_8009C984A[];
#define D_8009C984 D_8009C984A[0]
extern int D_8009C984s;
extern unsigned char D_8009C942, D_8009C93F, D_8009CDA7[];
extern unsigned short D_8009C960, D_8009C962;
extern int D_1F8002D4[];
extern void *D_8013B17C[];
extern void *D_8013B184, *D_8013B188, *D_8013B18C;
extern void *D_8013B184a[];
extern void *D_8013B18Ca[];
extern signed char D_80011EB4[];
extern E4 D_80011DAC[];
extern char D_80077D30[], D_80077D3C[], D_80077D18[];
extern int ObjCullRegister(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int AnimAdvance_(TObj *);
extern void applyFrameVelocityX(TObj *);
extern void ObjListPush_1F80021C(TObj *);
extern void freeObjectLayer2(TObj *);
extern void FUN_80133254(TObj *);
extern void func_80132F80(TObj *);
extern void func_800EEA7C(TObj *, int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void addItemToInventory(int, int, int);
extern void removeItemFromInventory(int, int);
extern void showMessageBox(int, int, int, int);
extern int MulCos(short a, int b);
extern int MulNegSinScaled(short a, int b);
extern void ObjSpawnType5(int, int, void *);
extern void FUN_800eb50c(int, int, int, int);
extern void FUN_80020490(TObj *);
extern void func_8004D620(int, int);
extern short TileCollideAt(TObj *, short, short);

void func_80133474(TObj *o)
{
    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            if (D_8009C984 & 6) {
                o->b04 = 3;
                break;
            }
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0x18;
            o->box3 = 0x20;
            o->d8c = 0;
            o->w1e = 0xb;
            o->b0d = 0;
            o->d3c = D_1F8002D4[0];
            o->anim = D_8013B184a[0];
            o->b0a = 2;
            AnimLoadDuration(o);
            o->step++;
            break;
        case 1:
            o->visible = 0;
            ObjListPush_1F80021C(o);
            break;
        }
        break;
    case 1:
        if (ObjCullRegister(o)) {
            switch (o->step) {
            case 0:
                FUN_80133254(o);
                break;
            case 1:
                func_80132F80(o);
                break;
            }
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0:
                D_800A6038.visible = 1;
                D_800A6038.d8c = 0;
                D_8009C984 |= 2;
                func_800EEA7C(&D_800A6038, 0xd, 0);
                o->timer = 2;
                if (D_8009CDA7[0] == 0) {
                    o->w78 = 1;
                    o->h->p.whole = D_800A6038.h->p.whole;
                    D_800A6038.y.p.whole = o->y.p.whole - o->box2;
                    setEventStarted(3, 0, 0);
                    o->timer = 300;
                    D_8009C942 = 1;
                    D_8009C93F = 1;
                }
                addItemToInventory(1, 1, 1);
                o->state++;
            case 1:
                if (--o->timer <= 0) {
                    D_8009C942 = 0;
                    D_8009C93F = 0;
                    o->step = 1;
                }
                break;
            }
            break;
        case 1:
            AnimAdvance(o);
            break;
        case 2:
            if (o->w78 != 0) {
                D_8009C942 = 0;
                D_8009C93F = 0;
            }
            D_8009C984 &= ~2;
            o->step = 5;
            o->state = 0;
            break;
        case 3: {
            E4 *e;
            unsigned short t;
            int ang;
            o->visible = D_800A6039;
            o->active = 2;
            o->animFrame = D_800A6066 & 1;
            o->category |= 0x80;
            e = &D_80011DAC[D_80011EB4[*D_800A605C[0]]];
            o->anim = D_8013B17C[e->anim];
            if (o->animFrame & 1) {
                t = D_800A60C4 + 0x80 - e->ang;
                t &= 0xff;
            } else {
                t = e->ang + D_800A60C4;
                t &= 0xff;
            }
            ang = (short)t;
            o->h->p.whole = D_800A6078->p.whole + MulCos(ang, e->rad);
            o->y.p.whole = D_800A6038.y.p.whole + MulNegSinScaled(ang, e->rad);
            o->d->p.whole = D_800A607Ca[0]->p.whole;
            o->d8c = D_800A60C4a[0];
            o->b0f = D_800A6047 + (unsigned char)e->z;
            if (D_800A603C == 2) {
                D_8009C984 &= ~2;
                o->b04 = 2;
                o->step = 5;
                o->state = 0;
            }
            if (D_8009C960 != 0) break;
            if (D_8009C962 == 0 && D_800A603D == 0x20 && D_800A607C->p.whole > 0 &&
                D_800A6038.y.p.whole >= -0x2f && (unsigned short)(D_800A6078->p.whole - 0x400) < 0x18) {
                D_8009C984s &= ~2;
                o->b04 = 2;
                o->step = 5;
                o->state = 0;
            }
            if (D_8009C984 & 4) break;
            if (!(D_8009C984 & 2)) break;
            if (D_800A609C != 0) {
                o->b04 = 2;
                o->step = 5;
                o->state = 0;
                break;
            }
            switch (D_8009C962) {
            case 2:
                if (D_800A6078->p.whole >= 0xa61) o->step = 6;
                break;
            case 5:
                if (D_800A6078->p.whole >= 0xbd && D_800A60DA == 0) {
                    o->step = 4;
                    D_8009C942 = 1;
                    D_8009C93F = 1;
                }
                break;
            }
            break;
        }
        case 4:
            switch (o->state) {
            case 1:
                goto L3ffc;
            case 0:
                o->active = 2;
                o->animFrame = 0;
                o->d8c = 0;
                removeItemFromInventory(1, 1);
                o->anim = D_8013B18C;
                AnimLoadDuration(o);
                o->movetab = D_80077D30;
                o->velY = -0x280;
                showMessageBox(5, 0, 0x100, 0x9c);
                D_800A6038.animFrame = 0;
                goto L4034;
            case 2: {
                V12 v;
                AnimAdvance(o);
                applyFrameVelocityX(o);
                o->y.raw += o->velY << 8;
                o->velY += 0x20;
                if (o->y.p.whole < -0xef) break;
                D_800A6038.visible = 1;
                v = *(V12 *)&o->a;
                ObjSpawnType5(0x10, 0, &v);
                ObjSpawnType5(0x10, 0, &v);
                setEventComplete(3, 0);
                FUN_800eb50c(1, o->a.p.whole, (short)(o->y.p.whole - 0x20), o->b.p.whole);
                D_8009C984 = (D_8009C984 | 4) & ~2;
                o->timer = 300;
                o->state++;
                break;
            }
            case 3:
                if (--o->timer <= 0) {
                    D_8009C942 = 0;
                    D_8009C93F = 0;
                    o->b04 = 3;
                }
                break;
            }
            break;
        case 5:
            if (o->visible == 0) o->b04 = 3;
            switch (o->state) {
            case 0:
                o->d8c = 0;
                o->active = 2;
                o->anim = D_8013B18Ca[0];
                AnimLoadDuration(o);
                removeItemFromInventory(1, 1);
                o->movetab = D_80077D3C;
                o->animFrame = !(D_800A6038.h->p.whole < o->h->p.whole);
                o->velY = -0x400;
                D_8009C984 &= ~2;
                func_8004D620(0x10, 2);
                o->state++;
            case 1:
                applyFrameVelocityX(o);
                AnimAdvance(o);
                goto L400c;
            case 2:
                applyFrameVelocityX(o);
                AnimAdvance(o);
                o->y.raw += o->velY << 8;
                o->velY += 0x20;
                if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8) == 0) break;
                o->anim = D_8013B188;
                AnimLoadDuration(o);
                goto L4034;
            case 3:
                applyFrameVelocityX(o);
                AnimAdvance(o);
                o->y.raw += o->velY << 8;
                o->velY += 0x20;
                if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8) == 0) break;
                goto L4034;
            case 4:
                if (AnimAdvance(o) == 0) break;
                goto L4034;
            case 5:
                o->anim = D_8013B17C[0];
                AnimLoadDuration(o);
                o->movetab = D_80077D3C;
                o->velY = -0x300;
                o->animFrame = !(D_800A6038.h->p.whole < o->h->p.whole);
                FUN_80020490(o);
                goto L4034;
            case 6:
                applyFrameVelocityX(o);
                AnimAdvance_(o);
                goto L400c;
            case 7:
                applyFrameVelocityX(o);
                AnimAdvance(o);
                o->y.raw += o->velY << 8;
                o->velY += 0x20;
                if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8) == 0) break;
                o->anim = D_8013B17C[0];
                AnimLoadDuration(o);
                o->state = 5;
                break;
            }
            break;
        case 6:
            switch (o->state) {
            case 1:
                goto L3ffc;
            case 0:
                o->active = 2;
                o->animFrame = 0;
                o->anim = D_8013B18Ca[0];
                AnimLoadDuration(o);
                o->movetab = D_80077D18;
                o->velY = -0x200;
                removeItemFromInventory(1, 1);
                D_8009C984 &= ~2;
                showMessageBox(5, 1, 0x100, 0x9c);
                goto L4034;
            L3ffc:
                AnimAdvance(o);
                applyFrameVelocityX(o);
            L400c:
                o->y.raw += o->velY << 8;
                o->velY += 0x20;
                if (o->velY <= 0) break;
            L4034:
                o->state++;
                break;
            case 2:
                AnimAdvance(o);
                applyFrameVelocityX(o);
                o->y.raw += o->velY << 8;
                o->velY += 0x20;
                if (TileCollideAt(o, o->h->p.whole, o->y.p.whole) == 0) break;
                o->b04 = 3;
                break;
            }
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
