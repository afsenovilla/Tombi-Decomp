// FUNC 8012415c 704 X014
// MATCHING 8012415c 704
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

#define SFX() do { int c = 0xda; if (D_8009C962 == 7) c = 0xf2; playSFX(c); } while (0)

void func_8012415C(TObj *o)
{
    unsigned char *t;
    int a;

    switch (o->b04) {
    case 0:
        o->b04++;
        t = &D_801266F0[o->subtype * 2];
        {
        int b;
        a = t[0];
        o->box0 = a;
        a <<= 1;
        o->box1 = a;
        b = t[1];
        o->box2 = b;
        b <<= 1;
        o->box3 = b;
        o->w1e = 1;
        o->d84 = 0;
        o->d88 = 0;
        }
        if (o->b0c == 1) {
            o->b0d = 0;
            o->w22 = 0;
            SFX();
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
                SFX();
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
