// FUNC 8012415c 704 X014
/* score 22: case 0 box setup: game keeps t[0] in v0 and t[1] in v1 (two block-local values shifted in place); here the
   shared `a` is a global pseudo and lands in a0. Two separate vars or block-local temps make it much worse (153).
   Rest of the function matches. */
#include "TOBJ.H"

extern unsigned char D_801266F0[];
extern unsigned short D_8009C962;
extern unsigned char D_8009C942;
extern int D_1F8002DC;
extern void *D_80129FA4[];
extern void playSFX(int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);
extern void func_80123D64(TObj *);
extern void func_80123AAC(TObj *);
extern void func_80123730(TObj *);

void func_8012415C(TObj *o)
{
    unsigned char *t;
    int a;

    switch (o->b04) {
    case 0:
        o->b04++;
        t = &D_801266F0[o->subtype * 2];
        a = t[0];
        o->box0 = a;
        a <<= 1;
        o->box1 = a;
        o->w1e = 1;
        o->d84 = 0;
        o->d88 = 0;
        a = t[1];
        o->box2 = a;
        a <<= 1;
        o->box3 = a;
        if (o->b0c == 1) {
            o->b0d = 0;
            o->w22 = 0;
            a = 0xda;
            if (D_8009C962 == 7) a = 0xf2;
            playSFX(a);
            o->w22++;
        } else {
            o->b0d = 0x80;
        }
        o->d3c = D_1F8002DC;
        o->anim = D_80129FA4[o->b0c];
        AnimLoadDuration(o);
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            if (D_8009C942 == 1) goto cull;
            ObjCullRegister(o);
            AnimAdvance(o);
            break;
        case 1:
            if (D_8009C942 == 1) {
            cull:
                ObjCullRegister(o);
                break;
            }
            if (!(o->w22 & 0x3f)) {
                a = 0xda;
                if (D_8009C962 == 7) a = 0xf2;
                playSFX(a);
            }
            o->w22++;
            func_80123D64(o);
            break;
        case 2:
            func_80123AAC(o);
            break;
        }
        break;
    case 2:
        if (o->subtype == 0) {
            ObjCullRegister(o);
            if (D_8009C942) break;
            AnimAdvance(o);
            o->velX -= 4;
            if (o->velX <= 0) {
                o->velX = 0;
                o->b04++;
            }
        } else if (o->subtype != 1) {
            o->b04++;
        } else {
            o->b04++;
            ((TObj *)o->d94)->b04 = 2;
            func_80123730(o);
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
